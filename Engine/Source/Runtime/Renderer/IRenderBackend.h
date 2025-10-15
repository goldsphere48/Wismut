#pragma once
#include "IRenderViewport.h"

namespace Wi
{
	class IRenderBackend
	{
	public:
		virtual ~IRenderBackend() = default;

		virtual IRenderViewport* CreateViewport(void* hwnd) const = 0;
		virtual void DestroyViewport(const IRenderViewport* viewport) const = 0;

		virtual void BeginRenderViewport(IRenderViewport* viewport) = 0;
		virtual void EndRenderViewport(IRenderViewport* viewport) = 0;
		virtual void ClearColor(float r, float g, float b) = 0;
	};
}
