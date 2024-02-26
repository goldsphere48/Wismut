#pragma once
#include <vulkan/vulkan.hpp>

#include "Wismut/Renderer/RendererCommon.h"

#define VK_CHECK_RESULT(result, msg) WI_CORE_ASSERT(result == vk::Result::eSuccess, msg)

namespace Wi::VulkanUtils
{
	template<typename T>
	void ZeroVulkanStruct(T& structure, vk::StructureType type)
	{
		structure.sType = type;
		memset(reinterpret_cast<char*>(&structure) + sizeof(type), 0, sizeof(T) - sizeof(type));
	}

	vk::ShaderStageFlagBits ConvertToVkShaderStage(ShaderStage stage);

	vk::VertexInputRate ConvertToVkInputRate(VertexInputRate frequency);

	vk::Format ConvertDataFormatToVkFormat(DataFormat format);

	vk::BufferUsageFlagBits ConvertToVkBufferUsage(BufferType usage);
}
