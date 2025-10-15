#pragma once

#include "Renderer/OpenGL/OpenGLPlatform.h"
#include "Renderer/IRenderViewport.h"

namespace Wi
{
	class GLViewport : public IRenderViewport
	{
	public:
		GLViewport(void* nativeHandler);
		~GLViewport() override;

		PlatformOpenGLContext* GetContext() const;

	private:
		PlatformOpenGLContext* m_Context = nullptr;
	};
}
