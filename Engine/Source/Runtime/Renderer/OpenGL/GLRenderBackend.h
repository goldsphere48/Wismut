#pragma once

#include "Renderer/IRenderBackend.h"

namespace Wi
{
	class GLRenderBackend : public IRenderBackend
	{
	public:
		GLRenderBackend();

		IRenderViewport* CreateViewport(void* hwnd) const override;
		void DestroyViewport(const IRenderViewport* viewport) const override;

		void BeginRenderViewport(IRenderViewport* viewport) override;
		void EndRenderViewport(IRenderViewport* viewport) override;
		void ClearColor(float r, float g, float b) override;
	};
}
