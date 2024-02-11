#include "wipch.h"
#include "VulkanRendererAPI.h"

#include <magic_enum/magic_enum.hpp>

#include "VulkanContext.h"
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
				.pName = stageConfig.Entry.c_str()
			};

			shader->VkStagesCreateInfo.push_back(stageCreateInfo);
		}

		WI_CORE_INFO("Shader created {0}", config.Name);

		return shader;
	}

	void VulkanRendererAPI::DestroyShaderProgram(const std::shared_ptr<Shader>& shader) const
	{
		const VulkanShader* vkShader = static_cast<VulkanShader*>(shader.get());
		for (const auto& stages : vkShader->VkStagesCreateInfo)
			m_Device.destroyShaderModule(stages.module);
	}
}
