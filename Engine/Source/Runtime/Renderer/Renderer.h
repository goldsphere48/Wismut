#pragma once
#include "IRenderBackend.h"
#include "IRenderViewport.h"
#include "Core/Memory/MemoryTypes.h"

namespace Wi
{
	struct PlatformWindowContext;

	class Renderer
	{
	public:
		static void Initialize();
		static void Shutdown();

		static IRenderViewport* CreateViewport(void* handle);
		static void DestroyViewport(const IRenderViewport* viewport);

		static const UniquePtr<IRenderBackend>& GetBackend()
		{
			return s_RenderBackend;
		}

	private:
		static UniquePtr<IRenderBackend> s_RenderBackend;
	};
}
