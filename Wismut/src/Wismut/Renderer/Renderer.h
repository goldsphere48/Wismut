#pragma once
#include "RendererAPI.h"
#include "RendererContext.h"

namespace Wi
{
	class Renderer
	{
	public:
		static void Initialize();
		static void Shutdown();

		static const std::unique_ptr<RendererContext>& GetContext() { return s_RendererContext; }

	private:
		inline static std::unique_ptr<RendererAPI> s_RenderAPI = nullptr;
		inline static std::unique_ptr<RendererContext> s_RendererContext = nullptr;
	};
}
