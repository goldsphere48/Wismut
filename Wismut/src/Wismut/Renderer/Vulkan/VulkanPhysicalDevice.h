#pragma once

#include <vulkan/vulkan.hpp>

namespace Wi
{
	struct VulkanQueueFamilyIndices
	{
		std::optional<uint32_t> Graphics;
		std::optional<uint32_t> Transfer;
		std::optional<uint32_t> Compute;
		std::optional<uint32_t> Present;

		bool IsComplete() const
		{
			return Graphics.has_value() && Transfer.has_value() && Compute.has_value();
		}
	};

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice(vk::PhysicalDevice device);

		bool IsSupportExtension(const std::string& extension) const;
		VulkanQueueFamilyIndices GetQueueFamilies() const;

	public:
		VulkanQueueFamilyIndices QueueFamilyIndices;
		vk::PhysicalDevice Device;
		std::vector<vk::QueueFamilyProperties> FamilyProperties;
		std::vector<vk::ExtensionProperties> DeviceSupportedExtensions;
		vk::PhysicalDeviceProperties Properties;
		vk::PhysicalDeviceMemoryProperties MemoryProperties;
		vk::PhysicalDeviceFeatures Features;
	};
}

