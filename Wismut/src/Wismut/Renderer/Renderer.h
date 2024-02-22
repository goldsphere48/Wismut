#pragma once
#include "RendererConfig.h"
#include "RendererAPI.h"
#include "RendererContext.h"
#include "ShaderLibrary.h"

namespace Wi
{
	class Renderer
	{
	public:
		static void Initialize(const RendererConfig& config);
		static void Shutdown();
		static void OnWindowResize(int width, int height);

		static std::shared_ptr<Buffer> CreateVertexBuffer(const std::vector<Vertex>& data);
		static std::shared_ptr<GraphicsPipeline> CreateGraphicsPipeline(const PipelineSpecification& specification);
		static std::shared_ptr<RenderPass> CreateRenderPass(const RenderPassSpecification& specification);
		static std::shared_ptr<Shader> CreateShaderProgram(const ShaderConfig& config);

		static void DestroyShaderProgram(const std::shared_ptr<Shader>& shader);

		static void BeginFrame();
		static void EndFrame();
		static void DrawTest();

		static const RendererContext* GetContext() { return s_RendererContext; }
		static const RendererAPI* GetApi() { return s_RenderAPI; }
		static const RendererConfig& GetConfig() { return s_RendererConfig; }
		static uint32_t GetCurrentFrameIndex();

	private:
		inline static std::shared_ptr<GraphicsPipeline> m_Pipeline;
		inline static std::shared_ptr<Buffer> m_Buffer;

		inline static ShaderLibrary s_Library;
		inline static RendererConfig s_RendererConfig;
		inline static const RendererAPI* s_RenderAPI = nullptr;
		inline static RendererContext* s_RendererContext = nullptr;
	};
}
