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

		static void Begin();
		static void End();
		static void OnWindowResize(int width, int height);

		static void DrawTest();

		static const RendererContext* GetContext() { return s_RendererContext; }
		static const RendererAPI* GetApi() { return s_RenderAPI; }
		static uint32_t GetCurrentFrameIndex();

	public:
		inline static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

	private:
		inline static std::shared_ptr<GraphicsPipeline> m_Pipeline;
		inline static std::shared_ptr<Buffer> m_Buffer;

		inline static ShaderLibrary s_Library;
		inline static const RendererAPI* s_RenderAPI = nullptr;
		inline static RendererContext* s_RendererContext = nullptr;
	};
}
