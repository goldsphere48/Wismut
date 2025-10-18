#ifdef WI_PLATFORM_WIN

#include <Windows.h>
#include <wingdi.h>

#include <glad/glad_wgl.h>

#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"
#include "Renderer/RenderConfig.h"
#include "Renderer/OpenGL/OpenGLPlatform.h"
#include "Renderer/OpenGL/GLUtils.h"
#include "Core/Utils/StringUtils.h"

namespace Wi
{
	struct PlatformOpenGLContext
	{
		HWND WindowHandle;
		HGLRC OpenGLContext = nullptr;
		HDC DeviceContext;
		bool VSync;
	};


	static const char* GExtensionsString;
	static bool GIsVSyncSupported;

	static bool IsVSyncExtensionSupported()
	{
		return Utils::HasSubstringInCString(GExtensionsString, "WGL_EXT_swap_control");
	}

	static void InitPixelFormatARB(HDC hdc)
	{
		const int pixelAttributes[] =
		{
			WGL_DRAW_TO_WINDOW_ARB,	GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,	GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
			WGL_PIXEL_TYPE_ARB,		WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,		32,
			WGL_DEPTH_BITS_ARB,		24,
			WGL_STENCIL_BITS_ARB,	8,
			WGL_ACCELERATION_ARB,	WGL_FULL_ACCELERATION_ARB,
			0,
		};

		int pixelFormat;
		UINT numFormats;
		BOOL result = wglChoosePixelFormatARB(hdc, pixelAttributes, nullptr, 1, &pixelFormat, &numFormats);
		WI_ASSERT(result && numFormats > 0, "Failed to choose pixel format")

		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);
		SetPixelFormat(hdc, pixelFormat, &pfd);
	}

	static void CreateFakeOpenGLWindow(PlatformOpenGLContext* outContext)
	{
		WNDCLASSA wc = {};
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = DefWindowProcA;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpszClassName = "FakeGLWindow";

		RegisterClassA(&wc);

		outContext->WindowHandle = CreateWindowA(
			"FakeGLWindow", "Fake",
			WS_OVERLAPPEDWINDOW,
			0, 0, 1, 1,
			nullptr, nullptr,
			GetModuleHandle(nullptr),
			nullptr
		);

		outContext->DeviceContext = GetDC(outContext->WindowHandle);
		WI_ASSERT(outContext->DeviceContext)

		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int format = ChoosePixelFormat(outContext->DeviceContext, &pfd);
		WI_ASSERT(SetPixelFormat(outContext->DeviceContext, format, &pfd))
	}

	static void CreateOpenGLCoreContext(PlatformOpenGLContext* context, int majorVersion, int minorVersion)
	{
		WI_ASSERT(context)
		WI_ASSERT(context->DeviceContext)

		int flags = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
		if (RenderConfig::IsDebugEnvironment())
			flags |= WGL_CONTEXT_DEBUG_BIT_ARB;

		const int contextAttributes[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB,	majorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB,	minorVersion,
			WGL_CONTEXT_FLAGS_ARB,			flags,
			WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		context->OpenGLContext = wglCreateContextAttribsARB(context->DeviceContext, nullptr, contextAttributes);
	}

	void PlatformInitOpenGL()
	{
		PlatformOpenGLContext dummyContext;
		CreateFakeOpenGLWindow(&dummyContext);
		dummyContext.OpenGLContext = wglCreateContext(dummyContext.DeviceContext);
		wglMakeCurrent(dummyContext.DeviceContext, dummyContext.OpenGLContext);

		WI_ASSERT(gladLoadWGL(dummyContext.DeviceContext))
		WI_ASSERT(gladLoadGL())

		GExtensionsString = wglGetExtensionsStringEXT();
		GIsVSyncSupported = IsVSyncExtensionSupported();

		auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		Log::Info("Windows OpenGL Initialized: Version: {}", version, vendor);

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(dummyContext.OpenGLContext);
		ReleaseDC(dummyContext.WindowHandle, dummyContext.DeviceContext);
		DestroyWindow(dummyContext.WindowHandle);
	}

	PlatformOpenGLContext* PlatformOpenGLCreateContext(void* hwnd)
	{
		WI_ASSERT(hwnd)

		PlatformOpenGLContext* context = new PlatformOpenGLContext;
		context->WindowHandle = static_cast<HWND>(hwnd);
		context->DeviceContext = GetDC(context->WindowHandle);
		WI_ASSERT(context->DeviceContext)

		InitPixelFormatARB(context->DeviceContext);
		CreateOpenGLCoreContext(context, 4, 6);

		WI_ASSERT(context->OpenGLContext)

		wglMakeCurrent(context->DeviceContext, context->OpenGLContext);
		Debug::OpenGL::InitDebugContext();
		wglMakeCurrent(nullptr, nullptr);

		return context;
	}

	void PlatformOpenGLDestroyContext(const PlatformOpenGLContext* context)
	{
		WI_ASSERT(context)

		ReleaseDC(context->WindowHandle, context->DeviceContext);
		HGLRC current = wglGetCurrentContext();
		if (context->OpenGLContext == current)
			wglMakeCurrent(nullptr, nullptr);

		wglDeleteContext(context->OpenGLContext);

		delete context;
		context = nullptr;
	}

	void PlatformOpenGLSwapBuffers(const PlatformOpenGLContext* context)
	{
		WI_ASSERT(context)
		WI_ASSERT(context->DeviceContext)

		::SwapBuffers(context->DeviceContext);
	}

	void PlatformOpenGLBeginRenderViewport(PlatformOpenGLContext* context)
	{
		WI_ASSERT(context)
		WI_ASSERT(context->DeviceContext)
		WI_ASSERT(context->OpenGLContext)

		wglMakeCurrent(context->DeviceContext, context->OpenGLContext);

		if (GIsVSyncSupported && RenderConfig::GetInstance().VSync != context->VSync)
		{
			context->VSync = RenderConfig::GetInstance().VSync;
			wglSwapIntervalEXT(context->VSync ? 1 : 0);
		}
	}
}
#endif
