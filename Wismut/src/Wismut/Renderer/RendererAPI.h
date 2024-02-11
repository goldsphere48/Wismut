#pragma once
#include "Buffer.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "Shader.h"

namespace Wi
{
	enum class RendererAPIType
	{
		None,
		Vulkan	
	};

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;
		RendererAPI(RendererAPIType renderAPIType) { s_CurrentAPIType = renderAPIType; }
		static RendererAPIType Current();

		virtual std::shared_ptr<Shader> CreateShaderProgram(const ShaderConfig& config) const = 0;
		virtual std::shared_ptr<GraphicsPipeline> CreateGraphicsPipeline(const PipelineSpecification& specification) const = 0;
		virtual std::shared_ptr<RenderPass> CreateRenderPass(const RenderPassSpecification& specification) const = 0;
		virtual std::shared_ptr<Buffer> CreateBuffer(uint32_t size, BufferUsageFlagBits bufferUsage) const = 0;

		virtual void DestroyShaderProgram(const std::shared_ptr<Shader>& shader) const = 0;
		virtual void DestroyGraphicsPipeline(const std::shared_ptr<GraphicsPipeline>& pipeline) const = 0;
		virtual void DestroyRenderPass(const std::shared_ptr<RenderPass>& specification) const = 0;
		virtual void DestroyBuffer(const std::shared_ptr<Buffer>& buffer) const = 0;

	private:
		inline static RendererAPIType s_CurrentAPIType;
	};
}

