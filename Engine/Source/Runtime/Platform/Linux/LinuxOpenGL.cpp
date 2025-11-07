#ifdef WI_PLATFORM_LINUX
#include "Renderer/OpenGL/OpenGLPlatform.h"

#include <glad/glad.h>
#include <glad/glad_glx.h>

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"
#include "Core/Utils/StringUtils.h"
#include "Renderer/RenderConfig.h"
#include "Renderer/OpenGL/GLUtils.h"

namespace Wi
{
	struct PlatformOpenGLContext
	{
		xcb_window_t WindowHandle = 0;
		Display* XDisplay = nullptr;
		GLXContext OpenGLContext = nullptr;
		bool VSync = 0;
	};

	static Display* GSharedDisplay = nullptr;
	static xcb_connection_t* GXcbConnection = nullptr;
	static const char* GExtensionsList;
	static GLint GMajorVersion;
	static GLint GMinorVersion;
	static bool GIsVSyncSupported;

	static bool IsVSyncExtensionSupported()
	{
		return Utils::HasSubstringInCString(GExtensionsList, "GLX_EXT_swap_control");
	}

	void PlatformInitOpenGL()
	{
		GSharedDisplay = XOpenDisplay(nullptr);
		WI_CHECK(GSharedDisplay, "Failed to open X display")

		GXcbConnection = XGetXCBConnection(GSharedDisplay);

		int screen = DefaultScreen(GSharedDisplay);
		WI_CHECK(gladLoadGLX(GSharedDisplay, screen))

		GExtensionsList = glXQueryExtensionsString(GSharedDisplay, screen);
		GIsVSyncSupported = IsVSyncExtensionSupported();

		int glxMajor, glxMinor;
		WI_CHECK(glXQueryVersion(GSharedDisplay, &glxMajor, &glxMinor), "Failed to query GLX version")

		static int visualAttributes[] = {
			GLX_RENDER_TYPE,	GLX_RGBA_BIT,
			GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
			GLX_DOUBLEBUFFER,	GL_TRUE,
			GLX_RED_SIZE,		8,
			GLX_GREEN_SIZE,		8,
			GLX_BLUE_SIZE,		8,
			GLX_ALPHA_SIZE,		8,
			GLX_DEPTH_SIZE,		24,
			GLX_STENCIL_SIZE,	8,
			None
		};

		int fbCount = 0;
		GLXFBConfig* fbConfigs = glXChooseFBConfig(
			GSharedDisplay,
			DefaultScreen(GSharedDisplay),
			visualAttributes,
			&fbCount
		);

		WI_CHECK(fbConfigs && fbCount > 0, "Failed to find suitable framebuffer config")

		GLXFBConfig fbc = fbConfigs[0];
		XVisualInfo* visualInfo = glXGetVisualFromFBConfig(GSharedDisplay, fbc);

		WI_CHECK(visualInfo, "Failed to get visual info")

		Window root = RootWindow(GSharedDisplay, visualInfo->screen);
		XSetWindowAttributes swa;
		swa.colormap = XCreateColormap(GSharedDisplay, root, visualInfo->visual, AllocNone);
		swa.event_mask = StructureNotifyMask;

		Window tempWindow = XCreateWindow(
			GSharedDisplay,
			root,
			0, 0,
			1, 1,
			0,
			visualInfo->depth,
			InputOutput,
			visualInfo->visual,
			CWColormap | CWEventMask,
			&swa
		);

		GLXContext tempContext = glXCreateContext(GSharedDisplay, visualInfo, nullptr, GL_TRUE);
		WI_CHECK(tempContext, "Failed to create temporary GL context")

		glXMakeCurrent(GSharedDisplay, tempWindow, tempContext);

		WI_CHECK(gladLoadGL(), "Failed to load OpenGL")

		glGetIntegerv(GL_MAJOR_VERSION, &GMajorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &GMinorVersion);

		const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		Log::Info("Linux OpenGL Initialized: Version: {}, Vendor: {}, Renderer: {}", version, vendor, renderer);

		glXMakeCurrent(GSharedDisplay, NULL, nullptr);
		glXDestroyContext(GSharedDisplay, tempContext);
		XDestroyWindow(GSharedDisplay, tempWindow);
		XFree(visualInfo);
		XFree(fbConfigs);
	}

	PlatformOpenGLContext* PlatformOpenGLCreateContext(void* hwnd)
	{
		PlatformOpenGLContext* context = new PlatformOpenGLContext;

		xcb_window_t window = *static_cast<xcb_window_t*>(hwnd);
		context->WindowHandle = window;
		context->XDisplay = GSharedDisplay;

		static constexpr int pixelAttributes[] =
		{
			GLX_X_RENDERABLE,	GL_TRUE,
			GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
			GLX_RENDER_TYPE,	GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
			GLX_DOUBLEBUFFER,	GL_TRUE,
			GLX_RED_SIZE,		8,
			GLX_GREEN_SIZE,		8,
			GLX_BLUE_SIZE,		8,
			GLX_ALPHA_SIZE,		8,
			GLX_DEPTH_SIZE,		24,
			GLX_STENCIL_SIZE,	8,
			GLX_CONFIG_CAVEAT,	GLX_NONE,
			None
		};

		int fbCount;
		GLXFBConfig* fbConfigs = glXChooseFBConfig(
			context->XDisplay,
			DefaultScreen(context->XDisplay),
			pixelAttributes,
			&fbCount
		);

		WI_CHECK(fbConfigs && fbCount > 0, "Failed to find suitable framebuffer config")

		xcb_get_window_attributes_cookie_t wc = xcb_get_window_attributes(GXcbConnection, window);
		xcb_get_window_attributes_reply_t* wa = xcb_get_window_attributes_reply(GXcbConnection, wc, nullptr);
		xcb_visualid_t wndVisual = wa->visual;

		GLXFBConfig chosen = nullptr;
		for (int i = 0; i < fbCount; ++i) {
			XVisualInfo* vi = glXGetVisualFromFBConfig(GSharedDisplay, fbConfigs[i]);
			if (vi && static_cast<xcb_visualid_t>(vi->visualid) == wndVisual) {
				chosen = fbConfigs[i];
				XFree(vi);
				break;
			}
			if (vi)
				XFree(vi);
		}

		int flags = GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
		if constexpr (RenderConfig::IsDebugEnvironment())
			flags |= GLX_CONTEXT_DEBUG_BIT_ARB;

		int contextAttributes[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, GMajorVersion,
			GLX_CONTEXT_MINOR_VERSION_ARB, GMinorVersion,
			GLX_CONTEXT_FLAGS_ARB, flags,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			None
		};

		context->OpenGLContext = glXCreateContextAttribsARB(context->XDisplay, chosen,nullptr, True, contextAttributes);

		XFree(fbConfigs);

		WI_CHECK(context->OpenGLContext, "Failed to create OpenGL context")

		glXMakeCurrent(context->XDisplay, context->WindowHandle, context->OpenGLContext);
		Debug::OpenGL::InitDebugContext();
		glXMakeCurrent(context->XDisplay, None, nullptr);

		return context;
	}

	void PlatformOpenGLDestroyContext(const PlatformOpenGLContext* context)
	{
		WI_ASSERT(context, "Invalid context")

		GLXContext current = glXGetCurrentContext();
		if (context->OpenGLContext == current)
			glXMakeCurrent(context->XDisplay, None, nullptr);

		if (context->OpenGLContext)
			glXDestroyContext(context->XDisplay, context->OpenGLContext);

		delete context;
	}

	void PlatformOpenGLSwapBuffers(const PlatformOpenGLContext* context)
	{
		WI_ASSERT(context, "Invalid context")
		WI_ASSERT(context->XDisplay, "Invalid display")

		glXSwapBuffers(context->XDisplay, context->WindowHandle);
	}

	void PlatformOpenGLBeginRenderViewport(PlatformOpenGLContext* context)
	{
		WI_ASSERT(context, "Invalid context")
		WI_ASSERT(context->XDisplay, "Invalid display")
		WI_ASSERT(context->OpenGLContext, "Invalid GL context")

		glXMakeCurrent(context->XDisplay, context->WindowHandle, context->OpenGLContext);

		if (GIsVSyncSupported && context->VSync != RenderConfig::GetInstance().VSync) {
			context->VSync = RenderConfig::GetInstance().VSync;
			glXSwapIntervalEXT(context->XDisplay, context->WindowHandle, context->VSync ? 1 : 0);
		}
	}
}

#endif
