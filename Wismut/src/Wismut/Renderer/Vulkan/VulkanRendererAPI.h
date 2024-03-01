#pragma once
#include <spirv_cross/spirv_cross.hpp>
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

		struct VulkanBuffer : BufferHandler
		{
			vk::Buffer VkBuffer;
			vk::DeviceMemory VkDeviceMemory;
			vk::BufferUsageFlags Usage;
			vk::MemoryPropertyFlags MemoryProperties;
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

		class ScopedCommandBuffer
		{
		public:
			ScopedCommandBuffer(VulkanDevice* device);
			~ScopedCommandBuffer();
			vk::CommandBuffer VkCommandBuffer;

		private:
			VulkanDevice* m_Device;
			vk::Fence m_Fence;
		};

	public:
		VulkanRendererAPI(VulkanContext* context);

		ShaderHandler* CreateShaderFromBinary(const ShaderBinary& binary, ShaderDescription& outDescription) const override;
		PipelineHandler* CreateGraphicsPipeline(const PipelineSpecification& specification) const override;
		RenderPassHandler* CreateRenderPass(const RenderPassSpecification& specification) const override;
		BufferHandler* CreateBuffer(uint32_t size, BufferType bufferType) const override;

		void CopyBuffer(BufferHandler* src, BufferHandler* dst, uint32_t size) const override;
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

		void BindPipeline(const std::shared_ptr<GraphicsPipeline>& pipeline) const override;
		void BindVertexBuffer(const std::shared_ptr<Buffer>& buffer) const override;
		void BindIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) const override;
		void DrawIndexed(uint32_t count) const override;

	private:
		VulkanVertexFormat CreateVulkanVertexFormat(const VertexFormat& format) const;
		ShaderStageDescription ReflectSpirv(const uint32_t* spirv, size_t count) const;

	private:
		VulkanContext* m_Context;
		vk::Device m_Device;
	};
}
