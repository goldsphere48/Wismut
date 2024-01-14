#include "wipch.h"
#include "VulkanPhysicalDevice.h"

namespace Wi
{
	VulkanPhysicalDevice::VulkanPhysicalDevice(vk::PhysicalDevice device) : Device(device)
	{
		Properties = device.getProperties();
		Features = device.getFeatures();
		MemoryProperties = device.getMemoryProperties();
		FamilyProperties = device.getQueueFamilyProperties();
		DeviceSupportedExtensions = device.enumerateDeviceExtensionProperties();
	}

	bool VulkanPhysicalDevice::IsSupportExtension(const std::string& extension) const
	{
		for (int i = 0; i < DeviceSupportedExtensions.size(); ++i)
			if (extension == DeviceSupportedExtensions[i].extensionName.data() == 0)
				return true;

		return false;
	}

	VulkanQueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilies() const
	{
		VulkanQueueFamilyIndices indices;

		uint32_t index = 0;
		for (auto family : FamilyProperties)
		{
			if (
				(family.queueFlags & vk::QueueFlagBits::eTransfer) &&
				(family.queueFlags & vk::QueueFlagBits::eCompute) == static_cast<vk::QueueFlagBits>(0) &&
				(family.queueFlags & vk::QueueFlagBits::eGraphics) == static_cast<vk::QueueFlagBits>(0)
				)
			{
				indices.Transfer = index;
				break;
			}

			index++;
		}

		index = 0;
		for (auto family : FamilyProperties)
		{
			if (
				family.queueFlags & vk::QueueFlagBits::eCompute &&
				(family.queueFlags & vk::QueueFlagBits::eGraphics) == static_cast<vk::QueueFlagBits>(0)
				)
			{
				indices.Compute = index;
				break;
			}
			index++;
		}

		index = 0;
		for (auto family : FamilyProperties)
		{
			if (family.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.Graphics = index;
				break;
			}
			index++;
		}

		index = 0;
		if (!indices.Transfer.has_value())
		{
			for (auto family : FamilyProperties)
			{
				if (family.queueFlags & vk::QueueFlagBits::eTransfer)
				{
					indices.Transfer = index;
					break;
				}
				index++;
			}
		}

		index = 0;
		if (!indices.Compute.has_value())
		{
			for (auto family : FamilyProperties)
			{
				if (family.queueFlags & vk::QueueFlagBits::eCompute)
				{
					indices.Compute = index;
					break;
				}
				index++;
			}
		}

		index = 0;
		if (!indices.Graphics.has_value())
		{
			for (auto family : FamilyProperties)
			{
				if (family.queueFlags & vk::QueueFlagBits::eGraphics)
				{
					indices.Graphics = index;
					break;
				}
				index++;
			}
		}

		return indices;
	}
}
