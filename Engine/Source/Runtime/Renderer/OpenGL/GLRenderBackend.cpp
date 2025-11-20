#ifdef WI_OPENGL_BUILTIN
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

	void GLRenderBackend::ClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
#endif
