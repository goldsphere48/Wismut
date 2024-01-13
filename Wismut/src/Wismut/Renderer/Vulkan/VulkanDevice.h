#pragma once

#include <vulkan/vulkan.hpp>

namespace Wi
{
	struct VulkanQueueFamilyIndices
	{
		std::optional<uint32_t> Graphics;
		std::optional<uint32_t> Transfer;
		std::optional<uint32_t> Compute;

		bool IsComplete() const
		{
			return Graphics.has_value() && Transfer.has_value() && Compute.has_value();
		}
	};

	class VulkanDevice
	{
	public:
		VulkanDevice(vk::Instance instance);
		~VulkanDevice();

		const vk::PhysicalDevice& GetPhysicalDevice() const { return m_vkPhysicalDevice; }
		const vk::Device& GetLogicalDevice() const { return m_vkLogicalDevice; }
		const VulkanQueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

	private:
		vk::PhysicalDevice m_vkPhysicalDevice;
		vk::Device m_vkLogicalDevice;
		VulkanQueueFamilyIndices m_QueueFamilyIndices;
	};
}

