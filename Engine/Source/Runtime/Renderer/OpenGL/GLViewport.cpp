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
}
