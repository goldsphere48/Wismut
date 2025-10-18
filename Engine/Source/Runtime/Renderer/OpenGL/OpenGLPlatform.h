#pragma once

namespace Wi
{
	struct PlatformOpenGLContext;

	void PlatformInitOpenGL();

	PlatformOpenGLContext* PlatformOpenGLCreateContext(void* hwnd);

	void PlatformOpenGLDestroyContext(const PlatformOpenGLContext* context);

	void PlatformOpenGLSwapBuffers(const PlatformOpenGLContext* context);

	void PlatformOpenGLBeginRenderViewport(PlatformOpenGLContext* context);
}
