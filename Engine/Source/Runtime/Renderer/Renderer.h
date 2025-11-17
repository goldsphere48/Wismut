#pragma once
#include "IRenderBackend.h"
#include "Core/Memory/MemoryTypes.h"

namespace Wi
{
	struct PlatformWindowContext;

	class Renderer
	{
	public:
		static void Initialize();
		static void Shutdown();

		static const UniquePtr<IRenderBackend>& GetBackend()
		{
			return s_RenderBackend;
		}

	private:
		static UniquePtr<IRenderBackend> s_RenderBackend;
	};
}
