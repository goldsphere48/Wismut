#pragma once
#include "RendererAPI.h"
#include "RendererContext.h"
#include "ShaderLibrary.h"

namespace Wi
{
	class Renderer
	{
	public:
		static void Initialize();
		static void Shutdown();

		static const RendererContext* GetContext() { return s_RendererContext; }
		static const RendererAPI* GetApi() { return s_RenderAPI; }

	private:
		inline static ShaderLibrary s_Library;
		inline static const RendererAPI* s_RenderAPI = nullptr;
		inline static RendererContext* s_RendererContext = nullptr;
	};
}
