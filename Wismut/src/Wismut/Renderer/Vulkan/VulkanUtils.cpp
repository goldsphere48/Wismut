#include "wipch.h"
#include "VulkanUtils.h"

namespace Wi::VulkanUtils
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

	vk::Format ConvertDataFormatToVkFormat(DataFormat format)
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
}