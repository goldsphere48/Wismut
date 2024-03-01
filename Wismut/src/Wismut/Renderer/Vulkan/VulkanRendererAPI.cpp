#include "wipch.h"
#include "VulkanRendererAPI.h"

#include <magic_enum/magic_enum.hpp>

#include "VulkanContext.h"
#include "VulkanShaderCompiler.h"
#include "VulkanUtils.h"

namespace Wi
{
	VulkanRendererAPI::ScopedCommandBuffer::ScopedCommandBuffer(VulkanDevice* device) : m_Device(device)
	{
		const vk::CommandBufferAllocateInfo allocateInfo {
			.sType = vk::StructureType::eCommandBufferAllocateInfo,
			.commandPool = m_Device->CommandPool,
			.level = vk::CommandBufferLevel::ePrimary,
			.commandBufferCount = 1,
		};

		const auto buffers = m_Device->LogicalDevice.allocateCommandBuffers(allocateInfo);
		VkCommandBuffer = buffers[0];

		const vk::CommandBufferBeginInfo beginInfo {
			.sType = vk::StructureType::eCommandBufferBeginInfo,
			.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
		};

		const vk::FenceCreateInfo fenceInfo {
			.sType = vk::StructureType::eFenceCreateInfo,
			.flags = vk::FenceCreateFlagBits::eSignaled
		};

		m_Fence = m_Device->LogicalDevice.createFence(fenceInfo);

		VkCommandBuffer.begin(beginInfo);
	}

	VulkanRendererAPI::ScopedCommandBuffer::~ScopedCommandBuffer()
	{
		VkCommandBuffer.end();

		const vk::SubmitInfo submitInfo {
			.sType = vk::StructureType::eSubmitInfo,
			.commandBufferCount = 1,
			.pCommandBuffers = &VkCommandBuffer,
		};

		m_Device->LogicalDevice.resetFences(m_Fence);
		m_Device->GetGraphicsQueue().submit(submitInfo, m_Fence);
		const vk::Result result = m_Device->LogicalDevice.waitForFences(m_Fence, VK_TRUE, UINT64_MAX);
		VK_CHECK_RESULT(result, "Failed to wait fances");

		m_Device->LogicalDevice.freeCommandBuffers(m_Device->CommandPool, VkCommandBuffer);
		m_Device->LogicalDevice.destroyFence(m_Fence);
		m_Device = nullptr;
		VkCommandBuffer = nullptr;
	}

	VulkanRendererAPI::VulkanRendererAPI(VulkanContext* context) :
		RendererAPI(RendererAPIType::Vulkan),
		m_Context(context),
		m_Device(context->GetDevice()->LogicalDevice)
	{
		
	}

	ShaderHandler* VulkanRendererAPI::CreateShaderFromBinary(const ShaderBinary& binary, ShaderDescription& outDescription) const
	{
		VulkanShader* shader = new VulkanShader;
		for (auto[stage, code] : binary)
		{
			vk::ShaderModuleCreateInfo moduleCreateInfo{
				.sType = vk::StructureType::eShaderModuleCreateInfo,
				.codeSize = code->GetSize(),
				.pCode = code->GetData<uint32_t>(),
			};

			vk::ShaderModule vkModule = m_Device.createShaderModule(moduleCreateInfo);
			WI_CORE_ASSERT(vkModule, "Failed to create shader module");

			vk::PipelineShaderStageCreateInfo stageCreateInfo {
				.sType = vk::StructureType::ePipelineShaderStageCreateInfo,
				.stage = VulkanUtils::ConvertToVkShaderStage(stage),
				.module = vkModule,
				.pName = "main"
			};

			shader->VkStagesCreateInfo.push_back(stageCreateInfo);

			SpirvShaderBinary* vulkanShaderBinary = static_cast<SpirvShaderBinary*>(code);
			const ShaderStageDescription description = ReflectSpirv(vulkanShaderBinary->GetData<uint32_t>(), vulkanShaderBinary->GetCount());
			outDescription[stage] = description;
		}

		const vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo {
			.sType = vk::StructureType::ePipelineLayoutCreateInfo,
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};

		shader->PipelineLayout = m_Device.createPipelineLayout(pipelineLayoutCreateInfo);

		return shader;
	}

	void VulkanRendererAPI::DestroyShaderProgram(ShaderHandler* shader) const
	{
		const VulkanShader* vkShader = shader->AsStatic<VulkanShader>();
		for (const vk::PipelineShaderStageCreateInfo& stages : vkShader->VkStagesCreateInfo)
			m_Device.destroyShaderModule(stages.module);

		m_Device.destroyPipelineLayout(vkShader->PipelineLayout);

		delete shader;
		shader = nullptr;
	}

	PipelineHandler* VulkanRendererAPI::CreateGraphicsPipeline(const PipelineSpecification& specification) const
	{
		const auto vkShader = static_cast<VulkanShader*>(specification.Shader->Handler);
		const VulkanVertexFormat vertexFormat = CreateVulkanVertexFormat(specification.VertexFormat);

		vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo {
			.sType = vk::StructureType::ePipelineVertexInputStateCreateInfo,
			.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexFormat.BindingDescriptions.size()),
			.pVertexBindingDescriptions = vertexFormat.BindingDescriptions.data(),
			.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexFormat.Attributes.size()),
			.pVertexAttributeDescriptions = vertexFormat.Attributes.data()
		};

		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo {
			.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo,
			.topology = vk::PrimitiveTopology::eTriangleList,
			.primitiveRestartEnable = VK_FALSE,
		};

		vk::PipelineViewportStateCreateInfo viewportStateCreateInfo {
			.sType = vk::StructureType::ePipelineViewportStateCreateInfo,
			.viewportCount = 1,
			.scissorCount = 1,
		};

		vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo {
			.sType = vk::StructureType::ePipelineRasterizationStateCreateInfo,
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = vk::PolygonMode::eFill,
			.cullMode = vk::CullModeFlagBits::eBack,
			.frontFace = vk::FrontFace::eClockwise,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
			.lineWidth = 1.0f
		};

		vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo {
			.sType = vk::StructureType::ePipelineMultisampleStateCreateInfo,
			.rasterizationSamples = vk::SampleCountFlagBits::e1,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 1.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE
		};

		vk::PipelineColorBlendAttachmentState colorBlendAttachmentState {
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = vk::BlendFactor::eOne,
			.dstColorBlendFactor = vk::BlendFactor::eZero,
			.colorBlendOp = vk::BlendOp::eAdd,
			.srcAlphaBlendFactor = vk::BlendFactor::eOne,
			.dstAlphaBlendFactor = vk::BlendFactor::eZero,
			.alphaBlendOp = vk::BlendOp::eAdd,
			.colorWriteMask = 
				vk::ColorComponentFlagBits::eR | 
				vk::ColorComponentFlagBits::eG | 
				vk::ColorComponentFlagBits::eB | 
				vk::ColorComponentFlagBits::eA
		};

		vk::PipelineColorBlendStateCreateInfo blendingStateCreateInfo {
			.sType = vk::StructureType::ePipelineColorBlendStateCreateInfo,
			.logicOpEnable = VK_FALSE,
			.logicOp = vk::LogicOp::eCopy,
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachmentState,
		};

		blendingStateCreateInfo.blendConstants[0] = 0.0f;
		blendingStateCreateInfo.blendConstants[1] = 0.0f;
		blendingStateCreateInfo.blendConstants[2] = 0.0f;
		blendingStateCreateInfo.blendConstants[3] = 0.0f;

		std::vector<vk::DynamicState> dynamicStates = {
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor,
			vk::DynamicState::eLineWidth
		};

		vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo {
			.sType = vk::StructureType::ePipelineDynamicStateCreateInfo,
			.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
			.pDynamicStates = dynamicStates.data()
		};

		vk::GraphicsPipelineCreateInfo pipelineCreateInfo {
			.sType = vk::StructureType::eGraphicsPipelineCreateInfo,
			.stageCount = static_cast<uint32_t>(vkShader->VkStagesCreateInfo.size()),
			.pStages = vkShader->VkStagesCreateInfo.data(),
			.pVertexInputState = &vertexInputCreateInfo,
			.pInputAssemblyState = &inputAssemblyCreateInfo,
			.pViewportState = &viewportStateCreateInfo,
			.pRasterizationState = &rasterizationStateCreateInfo,
			.pMultisampleState = &multisampleStateCreateInfo,
			.pDepthStencilState = nullptr,
			.pColorBlendState = &blendingStateCreateInfo,
			.pDynamicState = &dynamicStateCreateInfo,
			.layout = vkShader->PipelineLayout,
			.renderPass = m_Context->GetSwapchain()->GetVkRenderPass(),
			.subpass = 0,
			.basePipelineHandle = nullptr,
			.basePipelineIndex = -1
		};

		const vk::ResultValue<vk::Pipeline> pipelineResult = m_Device.createGraphicsPipeline(nullptr, pipelineCreateInfo);
		VK_CHECK_RESULT(pipelineResult.result, "Failed to create vulkan graphics pipelie");

		VulkanGraphicsPipeline* pipeline = new VulkanGraphicsPipeline();
		pipeline->VkPipeline = pipelineResult.value;

		return pipeline;
	}

	void VulkanRendererAPI::DestroyGraphicsPipeline(PipelineHandler* pipeline) const
	{
		m_Device.waitIdle();
		const vk::Pipeline vkPipeline = pipeline->AsStatic<VulkanGraphicsPipeline>()->VkPipeline;
		m_Device.destroyPipeline(vkPipeline);

		delete pipeline;
		pipeline = nullptr;
	}

	RenderPassHandler* VulkanRendererAPI::CreateRenderPass(const RenderPassSpecification& specification) const
	{
		vk::AttachmentDescription colorAttachment {
			.format = m_Context->GetSwapchain()->GetImageFormat(),
			.samples = vk::SampleCountFlagBits::e1,
			.loadOp = vk::AttachmentLoadOp::eClear,
			.storeOp = vk::AttachmentStoreOp::eStore,
			.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
			.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
			.initialLayout = vk::ImageLayout::eUndefined,
			.finalLayout = vk::ImageLayout::ePresentSrcKHR
		};

		vk::AttachmentReference colorAttachmentReference {
			.attachment = 0,
			.layout = vk::ImageLayout::eColorAttachmentOptimal
		};

		vk::SubpassDescription subpass {
			.colorAttachmentCount = 1,
			.pColorAttachments = &colorAttachmentReference
		};

		constexpr vk::SubpassDependency dependency {
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
			.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
			.srcAccessMask = vk::AccessFlagBits::eNone,
			.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
		};

		const vk::RenderPassCreateInfo renderPassCreateInfo {
			.sType = vk::StructureType::eRenderPassCreateInfo,
			.attachmentCount = 1,
			.pAttachments = &colorAttachment,
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &dependency
		};

		VulkanRenderPass* renderPass = new VulkanRenderPass;

		WI_CORE_ASSERT(renderPass, "Failed to create render pass")

		renderPass->VkRenderPass = m_Device.createRenderPass(renderPassCreateInfo);

		return renderPass;
	}

	void VulkanRendererAPI::DestroyRenderPass(RenderPassHandler* renderPass) const
	{
		m_Device.waitIdle();
		const auto vkRenderPass = renderPass->AsStatic<VulkanRenderPass>();
		m_Device.destroyRenderPass(vkRenderPass->VkRenderPass);

		delete renderPass;
		renderPass = nullptr;
	}

	VulkanRendererAPI::VulkanVertexFormat VulkanRendererAPI::CreateVulkanVertexFormat(const VertexFormat& format) const
	{
		VulkanVertexFormat vertexFormat;
		vertexFormat.BindingDescriptions.resize(format.Descriptions.size());
		vertexFormat.Attributes.resize(format.Attributes.size());

		for (uint32_t i = 0; i < format.Descriptions.size(); ++i)
		{
			vertexFormat.BindingDescriptions[i] = vk::VertexInputBindingDescription{
				.binding = i,
				.stride = format.Descriptions[i].stride,
				.inputRate = VulkanUtils::ConvertToVkInputRate(format.Descriptions[i].Rate)
			};
		}

		for (uint32_t i = 0; i < format.Attributes.size(); ++i)
		{
			const VertexAttribute& attribute = format.Attributes[i];
			const vk::VertexInputAttributeDescription attributeDescription{
				.location = attribute.location,
				.binding = attribute.binding,
				.format = VulkanUtils::ConvertDataFormatToVkFormat(attribute.Format),
				.offset = attribute.offset
			};

			vertexFormat.Attributes[i] = attributeDescription;
		}

		return vertexFormat;
	}

	ShaderStageDescription VulkanRendererAPI::ReflectSpirv(const uint32_t* spirv, size_t count) const
	{
		ShaderStageDescription data;
		const spirv_cross::Compiler compiler(spirv, count);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();
		for (const auto& uniformBuffer : resources.uniform_buffers)
		{
			ShaderUniform uniform;
			uniform.Type = UniformType::UniformBuffer;
			uniform.Binding = compiler.get_decoration(uniformBuffer.id, spv::DecorationBinding);
			data.Uniforms[uniform.Binding] = uniform;
		}
		return data;
	}

	BufferHandler* VulkanRendererAPI::CreateBuffer(uint32_t size, BufferType bufferType) const
	{
		VulkanBuffer* handler = new VulkanBuffer;

		switch (bufferType)
		{
			case BufferType::Vertex:
				handler->Usage = vk::BufferUsageFlagBits::eVertexBuffer |
					vk::BufferUsageFlagBits::eTransferSrc |
					vk::BufferUsageFlagBits::eTransferDst;

				handler->MemoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
				break;
			case BufferType::Index:
				handler->Usage = vk::BufferUsageFlagBits::eIndexBuffer |
					vk::BufferUsageFlagBits::eTransferSrc |
					vk::BufferUsageFlagBits::eTransferDst;

				handler->MemoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
				break;
			case BufferType::Staging:
				handler->Usage = vk::BufferUsageFlagBits::eTransferSrc;
				handler->MemoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | 
					vk::MemoryPropertyFlagBits::eHostCoherent;
				break;
			case BufferType::Uniform:
				handler->Usage = vk::BufferUsageFlagBits::eUniformBuffer;
				handler->MemoryProperties = vk::MemoryPropertyFlagBits::eHostVisible |
					vk::MemoryPropertyFlagBits::eHostCoherent;
				break;
		}

		const vk::BufferCreateInfo bufferCreateInfo {
			.sType = vk::StructureType::eBufferCreateInfo,
			.size = size,
			.usage = handler->Usage,
			.sharingMode = vk::SharingMode::eExclusive,
		};

		const vk::Buffer vkBuffer = m_Device.createBuffer(bufferCreateInfo);

		WI_CORE_ASSERT(vkBuffer, "Failed to create buffer");

		const vk::MemoryRequirements memoryRequirements = m_Device.getBufferMemoryRequirements(vkBuffer);
		const uint32_t memoryTypeIndex = m_Context->GetDevice()->PhysicalDevice->FindMemoryIndex(memoryRequirements.memoryTypeBits, handler->MemoryProperties);

		const vk::MemoryAllocateInfo allocationInfo {
			.sType = vk::StructureType::eMemoryAllocateInfo,
			.allocationSize = memoryRequirements.size,
			.memoryTypeIndex = memoryTypeIndex
		};

		const vk::DeviceMemory vkDeviceMemory = m_Device.allocateMemory(allocationInfo);

		WI_CORE_ASSERT(vkDeviceMemory, "Failed to allocate deviec memory");

		m_Device.bindBufferMemory(vkBuffer, vkDeviceMemory, 0);

		handler->VkBuffer = vkBuffer;
		handler->VkDeviceMemory = vkDeviceMemory;
		return handler;
	}

	void VulkanRendererAPI::CopyBuffer(BufferHandler* src, BufferHandler* dst, uint32_t size) const
	{
		const auto scopedCommandBuffer = ScopedCommandBuffer(m_Context->GetDevice().get());
		const vk::Buffer srcBuffer = src->AsStatic<VulkanBuffer>()->VkBuffer;
		const vk::Buffer dstBuffer = dst->AsStatic<VulkanBuffer>()->VkBuffer;

		const vk::BufferCopy bufferCopy {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = size
		};

		scopedCommandBuffer.VkCommandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &bufferCopy);
	}

	uint8_t* VulkanRendererAPI::MapBuffer(BufferHandler* handler, size_t size) const
	{
		const vk::DeviceMemory memory = handler->AsStatic<VulkanBuffer>()->VkDeviceMemory;
		void* ptr = nullptr;
		const vk::Result result = m_Device.mapMemory(memory, 0, size, static_cast<vk::MemoryMapFlags>(0), &ptr);
		VK_CHECK_RESULT(result, "Failed to load data to device memory");
		return static_cast<uint8_t*>(ptr);
	}

	void VulkanRendererAPI::UnmapBuffer(BufferHandler* handler) const
	{
		const vk::DeviceMemory memory = handler->AsStatic<VulkanBuffer>()->VkDeviceMemory;
		m_Device.unmapMemory(memory);
	}

	void VulkanRendererAPI::DestroyBuffer(BufferHandler* buffer) const
	{
		m_Device.waitIdle();
		const auto vkBuffer = buffer->AsStatic<VulkanBuffer>();
		m_Device.destroyBuffer(vkBuffer->VkBuffer);
		m_Device.freeMemory(vkBuffer->VkDeviceMemory);

		delete buffer;
		buffer = nullptr;
	}

	void VulkanRendererAPI::BeginRenderPass() const
	{
		const vk::Framebuffer framebuffer = m_Context->GetCurrentFrameBuffer();
		const vk::CommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();

		vk::ClearColorValue value = {};
		value.setFloat32({ 0.2f, 0.2f, 0.2f, 0.2f });
		vk::ClearValue clearColor = { .color = value };

		const vk::RenderPassBeginInfo renderPassBeginInfo {
			.sType = vk::StructureType::eRenderPassBeginInfo,
			.renderPass = m_Context->GetSwapchain()->GetVkRenderPass(),
			.framebuffer = framebuffer,
			.renderArea = {
				.offset = {0, 0},
				.extent = m_Context->GetSwapchain()->GetExtent()
			},
			.clearValueCount = 1,
			.pClearValues = &clearColor
		};

		const auto scissor = vk::Rect2D{
			.offset = {
				.x = 0,
				.y = 0
			},
			.extent = m_Context->GetSwapchain()->GetExtent(),
		};

		const float width = static_cast<float>(m_Context->GetSwapchain()->GetWidth());
		const float height = static_cast<float>(m_Context->GetSwapchain()->GetHeight());

		const auto viewport = vk::Viewport{
			.width = width,
			.height = height,
			.minDepth = 0,
			.maxDepth = 1
		};

		commandBuffer.setScissor(0, 1, &scissor);
		commandBuffer.setViewport(0, 1, &viewport);

		commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	}

	void VulkanRendererAPI::EndRenderPass() const
	{
		const vk::CommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
		commandBuffer.endRenderPass();
	}

	void VulkanRendererAPI::BeginFrame() const
	{
		m_Context->GetSwapchain()->BeginFrame();

		const vk::CommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
		constexpr vk::CommandBufferBeginInfo commandBufferBeginInfo {
			.sType = vk::StructureType::eCommandBufferBeginInfo,
			.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
			.pInheritanceInfo = nullptr
		};

		commandBuffer.begin(commandBufferBeginInfo);
	}

	void VulkanRendererAPI::EndFrame() const
	{
		const vk::CommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
		commandBuffer.end();
		m_Context->GetSwapchain()->Present();
	}

	void VulkanRendererAPI::BindPipeline(const std::shared_ptr<GraphicsPipeline>& pipeline) const
	{
		const vk::Pipeline vkPipeline = pipeline->Handler->AsStatic<VulkanGraphicsPipeline>()->VkPipeline;
		const vk::CommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vkPipeline);
	}

	void VulkanRendererAPI::BindVertexBuffer(const std::shared_ptr<Buffer>& buffer) const
	{
		const vk::CommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
		const VulkanBuffer* vkBuffer = buffer->Handler->AsStatic<VulkanBuffer>();
		constexpr vk::DeviceSize offsets = { 0 };
		commandBuffer.bindVertexBuffers(0, 1, &vkBuffer->VkBuffer, &offsets);
	}

	void VulkanRendererAPI::BindIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) const
	{
		const vk::CommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
		const VulkanBuffer* vkBuffer = buffer->Handler->AsStatic<VulkanBuffer>();
		constexpr vk::DeviceSize offsets = { 0 };
		commandBuffer.bindIndexBuffer(vkBuffer->VkBuffer, offsets, vk::IndexType::eUint32);
	}

	void VulkanRendererAPI::DrawIndexed(uint32_t count) const
	{
		const vk::CommandBuffer commandBuffer = m_Context->GetCurrentCommandBuffer();
		commandBuffer.drawIndexed(count, 1, 0, 0, 0);
	}
}
