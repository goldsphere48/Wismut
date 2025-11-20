#pragma once
#ifdef WI_OPENGL_BUILTIN

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

		void MakeCurrent() override;
		void Present() override;

	private:
		PlatformOpenGLContext* m_Context = nullptr;
	};
}
#endif
