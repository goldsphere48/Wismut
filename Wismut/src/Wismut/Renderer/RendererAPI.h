#pragma once
#include "RendererCommon.h"

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

		virtual ShaderHandler* CreateShaderProgram(const ShaderConfig& config) const = 0;
		virtual PipelineHandler* CreateGraphicsPipeline(const PipelineSpecification& specification) const = 0;
		virtual RenderPassHandler* CreateRenderPass(const RenderPassSpecification& specification) const = 0;
		virtual BufferHandler* CreateBuffer(uint32_t size, BufferType bufferUsage) const = 0;

		virtual void DestroyShaderProgram(ShaderHandler* shader) const = 0;
		virtual void DestroyGraphicsPipeline(PipelineHandler* pipeline) const = 0;
		virtual void DestroyRenderPass(RenderPassHandler* specification) const = 0;
		virtual void DestroyBuffer(BufferHandler* buffer) const = 0;

		virtual void CopyBuffer(BufferHandler* src, BufferHandler* dst, uint32_t size) const = 0;
		virtual uint8_t* MapBuffer(BufferHandler* handler, size_t size) const = 0;
		virtual void UnmapBuffer(BufferHandler* handler) const = 0;

		virtual void BeginRenderPass() const = 0;
		virtual void EndRenderPass() const = 0;

		virtual void BeginFrame() const = 0;
		virtual void EndFrame() const = 0;

		virtual void BindPipeline(const std::shared_ptr<GraphicsPipeline>& pipeline) const = 0;
		virtual void BindVertexBuffer(const std::shared_ptr<Buffer>& buffer) const = 0;
		virtual void BindIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) const = 0;
		virtual void DrawIndexed(uint32_t count) const = 0;

	private:
		inline static RendererAPIType s_CurrentAPIType;
	};
}

