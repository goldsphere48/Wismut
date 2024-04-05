#pragma once
#include "Material.h"
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
		static std::shared_ptr<IndexBuffer> CreateIndexBuffer(const std::vector<uint32_t>& data);
		static std::shared_ptr<UniformBuffer> CreateUniformBuffer(uint32_t size);
		static std::shared_ptr<GraphicsPipeline> CreateGraphicsPipeline(const PipelineSpecification& specification);
		static std::shared_ptr<RenderPass> CreateRenderPass(const RenderPassSpecification& specification);
		static std::shared_ptr<Shader> CreateShaderProgram(const ShaderBinary& binary);
		static std::shared_ptr<Material> CreateMaterial(const std::shared_ptr<Shader>& shader);

		static void UpdateUniformBuffer(const std::shared_ptr<UniformBuffer>& buffer, const void* data);
		static void DestroyUniformBuffer(const std::shared_ptr<UniformBuffer>& buffer);
		static void DestroyShaderProgram(const std::shared_ptr<Shader>& shader);

		static void BeginFrame();
		static void EndFrame();
		static void DrawTest();

		static const RendererContext* GetContext() { return s_RendererContext; }
		static const RendererAPI* GetApi() { return s_RenderAPI; }
		static const RendererConfig& GetConfig() { return s_RendererConfig; }
		static uint32_t GetCurrentFrameIndex();

	private:
		static void UpdateUBOTest();

		inline static std::shared_ptr<GraphicsPipeline> m_Pipeline;
		inline static std::shared_ptr<Buffer> m_Buffer;
		inline static std::shared_ptr<UniformBuffer> m_UniformBuffer;
		inline static std::shared_ptr<IndexBuffer> m_IndexBuffer;

		inline static ShaderLibrary s_Library;
		inline static RendererConfig s_RendererConfig;
		inline static const RendererAPI* s_RenderAPI = nullptr;
		inline static RendererContext* s_RendererContext = nullptr;
		inline static UniformBufferObject s_UBO = {};
		inline static uint32_t s_Width;
		inline static uint32_t s_Height;
	};
}
