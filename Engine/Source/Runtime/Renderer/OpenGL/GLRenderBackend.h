#pragma once
#ifdef WI_OPENGL_BUILTIN

#include "Renderer/IRenderBackend.h"

namespace Wi
{
	struct GLBufferHandler : BufferHandler
	{
		
	};

	class GLRenderBackend : public IRenderBackend
	{
	public:
		GLRenderBackend();

		IRenderViewport* CreateViewport(void* hwnd) const override;
		void DestroyViewport(const IRenderViewport* viewport) const override;

		void ClearColor(float r, float g, float b) override;
	};
}
#endif
