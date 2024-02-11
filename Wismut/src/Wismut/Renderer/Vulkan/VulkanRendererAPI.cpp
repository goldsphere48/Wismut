#include "wipch.h"
#include "VulkanRendererAPI.h"

#include <magic_enum/magic_enum.hpp>

#include "VulkanBuffer.h"
#include "VulkanContext.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanShader.h"
#include "VulkanShaderCompiler.h"

namespace Wi
{
	vk::ShaderStageFlagBits ConvertToVkShaderStage(ShaderStage stage)
	{
		switch (stage)
		{
			case ShaderStage::Vertex:
				return vk::ShaderStageFlagBits::eVertex;
			case ShaderStage::Fragment:
				return vk::ShaderStageFlagBits::eFragment;
		}

		WI_CORE_ASSERT(false, "Unknown shader stage");

		return static_cast<vk::ShaderStageFlagBits>(0);
	}

	vk::VertexInputRate ConvertToVkInputRate(VertexInputRate frequency)
	{
		switch (frequency)
		{
			case VertexInputRate::Instance:
				return vk::VertexInputRate::eInstance;
			case VertexInputRate::Vertex:
				return vk::VertexInputRate::eVertex;
		}

		WI_CORE_ASSERT(false, "Unknown VertexInputRate");

		return static_cast<vk::VertexInputRate>(0);
	}

	vk::Format ConvertToDataFormatToVkFormat(DataFormat format)
	{
		switch (format)
		{
			case DataFormat::Float:
				return vk::Format::eR32Sfloat;
			case DataFormat::Vec2:
				return vk::Format::eR32G32Sfloat;
			case DataFormat::Vec3:
				return vk::Format::eR32G32B32Sfloat;
			case DataFormat::Vec4:
				return vk::Format::eR32G32B32A32Sfloat;
		}

		WI_CORE_ASSERT(false, "Unknown DataFormat");

		return static_cast<vk::Format>(0);
	}

	vk::BufferUsageFlagBits ConvertToVkBufferUsage(BufferUsageFlagBits usage)
	{
		switch (usage)
		{
		case BufferUsageFlagBits::Vertex:
			return vk::BufferUsageFlagBits::eVertexBuffer;
		}

		WI_CORE_ASSERT(false, "Unknown Buffer Usage");

		return static_cast<vk::BufferUsageFlagBits>(0);
	}

	VulkanRendererAPI::VulkanRendererAPI(VulkanContext* context) :
		RendererAPI(RendererAPIType::Vulkan),
		m_Context(context),
		m_Device(context->GetDevice()->LogicalDevice)
	{
		
	}

	std::shared_ptr<Shader> VulkanRendererAPI::CreateShaderProgram(const ShaderConfig& config) const
	{
		WI_CORE_INFO("Creating shader {0} ...", config.Name);
		VulkanShaderCompiler compiler(config.Language);
		std::shared_ptr<VulkanShader> shader = std::make_shared<VulkanShader>();

		for (const ShaderStageConfig& stageConfig : config.Stages)
		{
			WI_CORE_INFO("Creating shader module {0} stage {1}", stageConfig.SourceFilePath, magic_enum::enum_name(stageConfig.Stage))
			const std::vector<uint32_t> code = compiler.CompileToSpv(stageConfig);
			vk::ShaderModuleCreateInfo moduleCreateInfo {
				.sType = vk::StructureType::eShaderModuleCreateInfo,
				.codeSize = code.size() * sizeof(uint32_t),
				.pCode = code.data(),
			};

			vk::ShaderModule vkModule = m_Device.createShaderModule(moduleCreateInfo);
			WI_CORE_ASSERT(vkModule, "Failed to create shader module");

			vk::PipelineShaderStageCreateInfo stageCreateInfo {
				.sType = vk::StructureType::ePipelineShaderStageCreateInfo,
				.stage = ConvertToVkShaderStage(stageConfig.Stage),
				.module = vkModule,
				.pName = "main"
			};

			shader->VkStagesCreateInfo.push_back(stageCreateInfo);
		}

		const vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo {
			.sType = vk::StructureType::ePipelineLayoutCreateInfo,
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};

		shader->PipelineLayout = m_Device.createPipelineLayout(pipelineLayoutCreateInfo);

		WI_CORE_INFO("Shader created {0}", config.Name);

		return shader;
	}

	void VulkanRendererAPI::DestroyShaderProgram(const std::shared_ptr<Shader>& shader) const
	{
		const VulkanShader* vkShader = static_cast<VulkanShader*>(shader.get());
		for (const auto& stages : vkShader->VkStagesCreateInfo)
			m_Device.destroyShaderModule(stages.module);

		m_Device.destroyPipelineLayout(vkShader->PipelineLayout);
	}

	std::shared_ptr<GraphicsPipeline> VulkanRendererAPI::CreateGraphicsPipeline(const PipelineSpecification& specification) const
	{
		const auto vkShader = static_cast<VulkanShader*>(specification.Shader);
		const auto vkRenderPass = static_cast<VulkanRenderPass*>(specification.RenderPass);

		const auto vertexFormat = CreateVulkanVertexFormat(specification.VertexFormat);

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
			.renderPass = vkRenderPass->VkRenderPass,
			.subpass = 0,
			.basePipelineHandle = nullptr,
			.basePipelineIndex = -1
		};

		const auto result = m_Device.createGraphicsPipeline(nullptr, pipelineCreateInfo);
		WI_CORE_ASSERT(result.result == vk::Result::eSuccess, "Failed to create vulkan graphics pipelie");

		std::shared_ptr<VulkanGraphicsPipeline> pipeline = std::make_shared<VulkanGraphicsPipeline>();
		pipeline->VkPipeline = result.value;

		return pipeline;
	}

	void VulkanRendererAPI::DestroyGraphicsPipeline(const std::shared_ptr<GraphicsPipeline>& pipeline) const
	{
		const auto vkPipeline = static_cast<VulkanGraphicsPipeline*>(pipeline.get())->VkPipeline;
		m_Device.destroyPipeline(vkPipeline);
	}

	std::shared_ptr<RenderPass> VulkanRendererAPI::CreateRenderPass(const RenderPassSpecification& specification) const
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

		vk::RenderPassCreateInfo renderPassCreateInfo {
			.sType = vk::StructureType::eRenderPassCreateInfo,
			.attachmentCount = 1,
			.pAttachments = &colorAttachment,
			.subpassCount = 1,
			.pSubpasses = &subpass
		};

		std::shared_ptr<VulkanRenderPass> renderPass = std::make_shared<VulkanRenderPass>();

		WI_CORE_ASSERT(renderPass, "Failed to create render pass")

		renderPass->VkRenderPass = m_Device.createRenderPass(renderPassCreateInfo);

		return renderPass;
	}

	void VulkanRendererAPI::DestroyRenderPass(const std::shared_ptr<RenderPass>& renderPass) const
	{
		const auto vkRenderPass = static_cast<VulkanRenderPass*>(renderPass.get());
		m_Device.destroyRenderPass(vkRenderPass->VkRenderPass);
	}

	VulkanRendererAPI::VulkanVertexFormat VulkanRendererAPI::CreateVulkanVertexFormat(const VertexFormat& format) const
	{
		VulkanVertexFormat vertexFormat;
		vertexFormat.BindingDescriptions.resize(format.size());
		vertexFormat.Attributes.resize(format.size());

		uint32_t i = 0;
		for (const auto& vertexInfo : format)
		{
			const vk::VertexInputBindingDescription bindingDescription {
				.binding = 0,
				.stride = vertexInfo.stride,
				.inputRate = ConvertToVkInputRate(vertexInfo.Rate)
			};

			const vk::VertexInputAttributeDescription attributeDescription{
				.location = vertexInfo.location,
				.binding = 0,
				.format = ConvertToDataFormatToVkFormat(vertexInfo.Format),
				.offset = vertexInfo.offset
			};

			vertexFormat.BindingDescriptions[i] = bindingDescription;
			vertexFormat.Attributes[i] = attributeDescription;

			i++;
		}

		return vertexFormat;
	}

	std::shared_ptr<Buffer> VulkanRendererAPI::CreateBuffer(uint32_t size, BufferUsageFlagBits bufferUsage) const
	{
		const vk::BufferCreateInfo bufferCreateInfo {
			.sType = vk::StructureType::eBufferCreateInfo,
			.size = size,
			.usage = ConvertToVkBufferUsage(bufferUsage),
			.sharingMode = vk::SharingMode::eExclusive,
		};

		const vk::Buffer vkBuffer = m_Device.createBuffer(bufferCreateInfo);

		WI_CORE_ASSERT(vkBuffer, "Failed to create buffer");

		const vk::MemoryRequirements memoryRequirements = m_Device.getBufferMemoryRequirements(vkBuffer);
		const auto memoryFlags = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
		const auto memoryTypeIndex = m_Context->GetDevice()->PhysicalDevice->GetSuitableMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryFlags);

		const vk::MemoryAllocateInfo allocationInfo {
			.sType = vk::StructureType::eMemoryAllocateInfo,
			.allocationSize = memoryRequirements.size,
			.memoryTypeIndex = memoryTypeIndex
		};

		const vk::DeviceMemory vkDeviceMemory = m_Device.allocateMemory(allocationInfo);

		WI_CORE_ASSERT(vkDeviceMemory, "Failed to allocate deviec memory");

		m_Device.bindBufferMemory(vkBuffer, vkDeviceMemory, 0);

		std::shared_ptr<VulkanBuffer> buffer = std::make_shared<VulkanBuffer>();
		buffer->VkBuffer = vkBuffer;
		buffer->VkDeviceMemory= vkDeviceMemory;

		return buffer;
	}


	void VulkanRendererAPI::DestroyBuffer(const std::shared_ptr<Buffer>& buffer) const
	{
		const auto vkBuffer = static_cast<VulkanBuffer*>(buffer.get());
		m_Device.destroyBuffer(vkBuffer->VkBuffer);
		m_Device.freeMemory(vkBuffer->VkDeviceMemory);
	}
}
