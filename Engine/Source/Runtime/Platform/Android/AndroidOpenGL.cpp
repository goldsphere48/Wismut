#ifdef WI_PLATFORM_ANDROID

#include <android/native_window.h>
#include <glad/glad_egl.h>
#include <glad/glad.h>

#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"
#include "Renderer/RenderConfig.h"
#include "Renderer/OpenGL/OpenGLPlatform.h"
#include "Renderer/OpenGL/GLUtils.h"

namespace Wi
{
	struct PlatformOpenGLContext
	{
		ANativeWindow* NativeWindow = nullptr;
		EGLDisplay Display = EGL_NO_DISPLAY;
		EGLSurface Surface = EGL_NO_SURFACE;
		EGLContext Context = EGL_NO_CONTEXT;
		EGLConfig Config = nullptr;
		bool VSync = false;
	};

	static int GGlesVersion = 0;

	void PlatformInitOpenGL()
	{
		EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		WI_ASSERT(display != EGL_NO_DISPLAY, "Failed to get EGL display");

		int major = 0;
		int minor = 0;
		WI_ASSERT(eglInitialize(display, &major, &minor), "Failed to initialize EGL");

		Log::Info("EGL Initialized: Version {}.{}", major, minor);

		WI_ASSERT(eglBindAPI(EGL_OPENGL_ES_API), "Failed to bind OpenGL ES API")

		EGLint numConfigs;
		EGLConfig tempConfig;

		constexpr EGLint gles3Attributes[] =
		{
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
			EGL_NONE
		};

		GGlesVersion = 3;
		if (!eglChooseConfig(display, gles3Attributes, &tempConfig, 1, &numConfigs) || numConfigs == 0)
		{
			constexpr EGLint gles2Attributes[] =
			{
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_NONE
			};

			if (eglChooseConfig(display, gles2Attributes, &tempConfig, 1, &numConfigs) && numConfigs > 0)
				GGlesVersion = 2;
			else
				WI_ASSERT(false, "Failed to select gles config")
		}

		const EGLint pBufferAttributes[] =
		{
			EGL_WIDTH,	1,
			EGL_HEIGHT, 1,
			EGL_NONE
		};

		EGLSurface tempSurface = eglCreatePbufferSurface(display, tempConfig, pBufferAttributes);
		WI_ASSERT(tempSurface != EGL_NO_SURFACE, "Failed to create temporary surface")

		const EGLint contextAttributes[] =
		{
			EGL_CONTEXT_CLIENT_VERSION, GGlesVersion,
			EGL_NONE
		};

		EGLContext tempContext = eglCreateContext(display, tempConfig, EGL_NO_CONTEXT, contextAttributes);
		if (tempContext == EGL_NO_CONTEXT)
		{
			eglDestroySurface(display, tempSurface);
			WI_ASSERT(false, "Failed to create OpenGL ES context")
			return;
		}

		if (!eglMakeCurrent(display, tempSurface, tempSurface, tempContext))
		{
			Log::Error("Failed to make context current");
			eglDestroyContext(display, tempContext);
			eglDestroySurface(display, tempSurface);
			return;
		}

		if (!gladLoadGLES2Loader((GLADloadproc)eglGetProcAddress))
		{
			WI_ASSERT(false, "Failed to load OpenGL ES functions")
			return;
		}

		const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		Log::Info("Android OpenGL ES Initialized:");
		Log::Info("  Version: {}", version);
		Log::Info("  Vendor: {}", vendor);
		Log::Info("  Renderer: {}", renderer);

		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(display, tempContext);
		eglDestroySurface(display, tempSurface);
	}

	PlatformOpenGLContext* PlatformOpenGLCreateContext(void* hwnd)
	{
		WI_ASSERT(hwnd, "Invalid window handle");
		PlatformOpenGLContext* context = new PlatformOpenGLContext;
		context->NativeWindow = static_cast<ANativeWindow*>(hwnd);
		context->Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		if (!eglInitialize(context->Display, nullptr, nullptr))
		{
			delete context;
			WI_ASSERT(false, "Failed to initialize EGL");
			return nullptr;
		}

		const EGLint configAttributes[] = {
			EGL_RENDERABLE_TYPE, (GGlesVersion == 3) ? EGL_OPENGL_ES3_BIT : EGL_OPENGL_ES2_BIT,
			EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
			EGL_BLUE_SIZE,       8,
			EGL_GREEN_SIZE,      8,
			EGL_RED_SIZE,        8,
			EGL_ALPHA_SIZE,      8,
			EGL_DEPTH_SIZE,      24,
			EGL_STENCIL_SIZE,    8,
			EGL_NONE
		};

		EGLint numConfigs;
		if (!eglChooseConfig(context->Display, configAttributes, &context->Config, 1, &numConfigs) || numConfigs == 0)
		{
			delete context;
			WI_ASSERT(false, "Failed to choose EGL config")
			return nullptr;
		}

		EGLint format;
		eglGetConfigAttrib(context->Display, context->Config, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry(context->NativeWindow, 0, 0, format);

		constexpr EGLint surfaceAttributes[] = {
			EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
			EGL_NONE
		};

		context->Surface = eglCreateWindowSurface(context->Display, context->Config, context->NativeWindow, surfaceAttributes);
		if (context->Surface == EGL_NO_SURFACE)
		{
			delete context;
			WI_ASSERT(false, "Failed to create window surface")
			return nullptr;
		}

		bool debugSupported = false;
		if constexpr (RenderConfig::IsDebugEnvironment())
		{
			const char* extensions = eglQueryString(context->Display, EGL_EXTENSIONS);
			if (extensions && strstr(extensions, "EGL_KHR_debug"))
			{
				debugSupported = true;
			}
		}

		if (debugSupported)
		{
			const EGLint debugContextAttributes[] =
			{
				EGL_CONTEXT_CLIENT_VERSION, GGlesVersion,
				EGL_CONTEXT_OPENGL_DEBUG,   EGL_TRUE,
				EGL_NONE
			};
			context->Context = eglCreateContext(context->Display, context->Config, EGL_NO_CONTEXT, debugContextAttributes);
		}
		else
		{
			const EGLint contextAttributes[] =
			{
				EGL_CONTEXT_CLIENT_VERSION, GGlesVersion,
				EGL_NONE
			};
			context->Context = eglCreateContext(context->Display, context->Config, EGL_NO_CONTEXT, contextAttributes);
		}

		if (context->Context == EGL_NO_CONTEXT)
		{
			eglDestroySurface(context->Display, context->Surface);
			delete context;
			WI_ASSERT(false, "Failed to create EGL context")
			return nullptr;
		}

		eglMakeCurrent(context->Display, context->Surface, context->Surface, context->Context);

		if (debugSupported)
			Debug::OpenGL::InitDebugContext();

		eglMakeCurrent(context->Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		return context;
	}

	void PlatformOpenGLDestroyContext(const PlatformOpenGLContext* context)
	{
		WI_ASSERT(context, "Invalid context");

		EGLContext current = eglGetCurrentContext();
		if (context->Context == current)
		{
			eglMakeCurrent(context->Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		}

		if (context->Context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(context->Display, context->Context);
		}

		if (context->Surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(context->Display, context->Surface);
		}

		delete context;
	}

	void PlatformOpenGLSwapBuffers(const PlatformOpenGLContext* context)
	{
		WI_ASSERT(context);
		WI_ASSERT(context->Display != EGL_NO_DISPLAY);
		WI_ASSERT(context->Surface != EGL_NO_SURFACE);

		eglSwapBuffers(context->Display, context->Surface);
	}

	void PlatformOpenGLBeginRenderViewport(PlatformOpenGLContext* context)
	{
		WI_ASSERT(context);
		WI_ASSERT(context->Display != EGL_NO_DISPLAY);
		WI_ASSERT(context->Surface != EGL_NO_SURFACE);
		WI_ASSERT(context->Context != EGL_NO_CONTEXT);

		if (!eglMakeCurrent(context->Display, context->Surface, context->Surface, context->Context))
		{
			Log::Error("Failed to make context current");
			return;
		}

		if (context->VSync != RenderConfig::GetInstance().VSync) {
			context->VSync = RenderConfig::GetInstance().VSync;
			eglSwapInterval(context->Display, context->VSync ? 1 : 0);
		}
	}
}

#endif // WI_PLATFORM_ANDROID
