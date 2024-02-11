#pragma once
#include <vulkan/vulkan.hpp>

#include "Wismut/Renderer/Pipeline.h"

namespace Wi
{
	struct VulkanGraphicsPipeline : GraphicsPipeline
	{
		vk::Pipeline VkPipeline;
	};
}
