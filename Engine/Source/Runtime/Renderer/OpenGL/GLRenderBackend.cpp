#include "GLRenderBackend.h"

#include "GLViewport.h"
#include "OpenGLPlatform.h"
#include "Core/Logger/Logger.h"
#include <glad/glad.h>

namespace Wi
{
	GLRenderBackend::GLRenderBackend()
	{
		PlatformInitOpenGL();
	}

	IRenderViewport* GLRenderBackend::CreateViewport(void* hwnd) const
	{
		return new GLViewport(hwnd);
	}

	void GLRenderBackend::DestroyViewport(const IRenderViewport* viewport) const
	{
		delete viewport;
	}

	void GLRenderBackend::BeginRenderViewport(IRenderViewport* viewport)
	{
		GLViewport* glView = static_cast<GLViewport*>(viewport);

		PlatformOpenGLBeginRenderViewport(glView->GetContext());
	}

	void GLRenderBackend::EndRenderViewport(IRenderViewport* viewport)
	{
		GLViewport* glView = static_cast<GLViewport*>(viewport);

		PlatformOpenGLSwapBuffers(glView->GetContext());
	}

	void GLRenderBackend::ClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
