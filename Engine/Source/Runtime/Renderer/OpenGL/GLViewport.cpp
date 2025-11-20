#ifdef WI_OPENGL_BUILTIN
#include "GLViewport.h"

#include "Core/Assertion.h"

namespace Wi
{
	GLViewport::GLViewport(void* nativeHandler)
	{
		m_Context = PlatformOpenGLCreateContext(nativeHandler);
		WI_ASSERT(m_Context)
	}

	GLViewport::~GLViewport()
	{
		PlatformOpenGLDestroyContext(m_Context);
	}

	PlatformOpenGLContext* GLViewport::GetContext() const
	{
		return m_Context;
	}

	void GLViewport::MakeCurrent()
	{
		PlatformOpenGLBeginRenderViewport(m_Context);
	}

	void GLViewport::Present()
	{
		PlatformOpenGLSwapBuffers(m_Context);
	}
}
#endif
