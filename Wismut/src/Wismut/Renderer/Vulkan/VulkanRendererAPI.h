#pragma once
#include "VulkanDevice.h"
#include "Wismut/Renderer/RendererAPI.h"

namespace Wi
{
	class VulkanContext;

	class VulkanRendererAPI : public RendererAPI
	{
		struct VulkanVertexFormat
		{
			std::vector<vk::VertexInputBindingDescription> BindingDescriptions;
			std::vector<vk::VertexInputAttributeDescription> Attributes;
		};
	public:
		VulkanRendererAPI(VulkanContext* context);

		std::shared_ptr<Shader> CreateShaderProgram(const ShaderConfig& config) const override;
		std::shared_ptr<GraphicsPipeline> CreateGraphicsPipeline(const PipelineSpecification& specification) const override;
		std::shared_ptr<RenderPass> CreateRenderPass(const RenderPassSpecification& specification) const override;
		std::shared_ptr<Buffer> CreateBuffer(uint32_t size, BufferUsageFlagBits bufferUsage) const override;

		void DestroyShaderProgram(const std::shared_ptr<Shader>& shader) const override;
		void DestroyGraphicsPipeline(const std::shared_ptr<GraphicsPipeline>& pipeline) const override;
		void DestroyRenderPass(const std::shared_ptr<RenderPass>& renderPass) const override;
		void DestroyBuffer(const std::shared_ptr<Buffer>& buffer) const override;

	private:
		VulkanVertexFormat CreateVulkanVertexFormat(const VertexFormat& format) const;

	public:

	private:
		VulkanContext* m_Context;
		vk::Device m_Device;
	};
}
