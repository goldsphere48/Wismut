#pragma once

namespace Wi
{
	struct PlatformOpenGLContext;

	void PlatformInitOpenGL();

	PlatformOpenGLContext* PlatformCreateOpenGLContext(void* hwnd);

	void PlatformDestroyOpenGLContext(const PlatformOpenGLContext* context);

	void PlatformSwapBuffers(const PlatformOpenGLContext* context);

	void PlatformBeginRenderViewport(const PlatformOpenGLContext* context);
}
