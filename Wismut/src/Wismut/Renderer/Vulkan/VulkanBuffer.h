#pragma once
#include <vulkan/vulkan.hpp>

#include "Wismut/Renderer/Buffer.h"

namespace Wi
{
	struct VulkanBuffer : Buffer
	{
		vk::Buffer VkBuffer;
		vk::DeviceMemory VkDeviceMemory;
	};	
}
