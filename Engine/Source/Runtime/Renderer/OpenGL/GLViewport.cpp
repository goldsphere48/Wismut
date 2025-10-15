#include "GLViewport.h"

#include "Core/Assertion.h"

namespace Wi
{
	GLViewport::GLViewport(void* nativeHandler)
	{
		m_Context = PlatformCreateOpenGLContext(nativeHandler);
		WI_ASSERT(m_Context)
	}

	GLViewport::~GLViewport()
	{
		PlatformDestroyOpenGLContext(m_Context);
	}

	PlatformOpenGLContext* GLViewport::GetContext() const
	{
		return m_Context;
	}
}
