#pragma once
#include "VulkanDevice.h"
#include "Wismut/Renderer/RendererAPI.h"

namespace Wi
{
	class VulkanContext;

	class VulkanRendererAPI : public RendererAPI
	{
	private:
		struct VulkanVertexFormat
		{
			std::vector<vk::VertexInputBindingDescription> BindingDescriptions;
			std::vector<vk::VertexInputAttributeDescription> Attributes;
		};

		struct VulkanBuffer : BufferHandler
		{
			vk::Buffer VkBuffer;
			vk::DeviceMemory VkDeviceMemory;
		};

		struct VulkanGraphicsPipeline : PipelineHandler
		{
			vk::Pipeline VkPipeline;
		};

		struct VulkanRenderPass : RenderPassHandler
		{
			vk::RenderPass VkRenderPass;
		};

		struct VulkanShader : ShaderHandler
		{
			std::vector<vk::PipelineShaderStageCreateInfo> VkStagesCreateInfo;
			vk::PipelineLayout PipelineLayout;
		};

	public:
		VulkanRendererAPI(VulkanContext* context);

		ShaderHandler* CreateShaderProgram(const ShaderConfig& config) const override;
		PipelineHandler* CreateGraphicsPipeline(const PipelineSpecification& specification) const override;
		RenderPassHandler* CreateRenderPass(const RenderPassSpecification& specification) const override;
		BufferHandler* CreateBuffer(uint32_t size, BufferUsageFlagBits bufferUsage) const override;

		uint8_t* MapBuffer(BufferHandler* handler, size_t size) const override;
		void UnmapBuffer(BufferHandler* handler) const override;

		void DestroyShaderProgram(ShaderHandler* shader) const override;
		void DestroyGraphicsPipeline(PipelineHandler* pipeline) const override;
		void DestroyRenderPass(RenderPassHandler* renderPass) const override;
		void DestroyBuffer(BufferHandler* buffer) const override;

		void BeginRenderPass() const override;
		void EndRenderPass() const override;

		void BeginFrame() const override;
		void EndFrame() const override;

		void RenderGeometry(const std::shared_ptr<GraphicsPipeline>& pipeline, const std::shared_ptr<Buffer>& buffer) const override;

	private:
		VulkanVertexFormat CreateVulkanVertexFormat(const VertexFormat& format) const;

	private:
		VulkanContext* m_Context;
		vk::Device m_Device;
	};
}
