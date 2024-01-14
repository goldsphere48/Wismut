#pragma once
#include <vulkan/vulkan.hpp>

namespace Wi
{
	#define VK_CHECK_RESULT(result, msg) WI_CORE_ASSERT(result == vk::Result::eSuccess, msg)

	template<typename T>
	void ZeroVulkanStruct(T& structure, vk::StructureType type)
	{
		structure.sType = type;
		memset(reinterpret_cast<char*>(&structure) + sizeof(type), 0, sizeof(T) - sizeof(type));
	}
}