#pragma once
#include <vulkan/vulkan.hpp>
#include "Wismut/Renderer/Shader.h"

namespace Wi
{
	struct VulkanShader : Shader
	{
		std::vector<vk::PipelineShaderStageCreateInfo> VkStagesCreateInfo;
	};
}
