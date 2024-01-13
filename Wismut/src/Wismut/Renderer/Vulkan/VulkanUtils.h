#pragma once
#include <vulkan/vulkan.hpp>

namespace Wi
{
	#define VK_CHECK_RESULT(result, msg) WI_CORE_ASSERT(result == vk::Result::eSuccess, msg)

	template<typename T>
	T ZeroVulkanStructure(vk::StructureType type)
	{
		T structure;
		structure.sType = type;
		memset(&structure + sizeof(type), 0, sizeof(T));
		return structure;
	}
}