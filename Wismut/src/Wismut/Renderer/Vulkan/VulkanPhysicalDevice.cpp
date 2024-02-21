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

	VulkanQueueFamilyIndices VulkanPhysicalDevice::GetQueueFamilies(vk::SurfaceKHR surface) const
	{
		VulkanQueueFamilyIndices indices;

		uint32_t index = 0;
		for (vk::QueueFamilyProperties family : FamilyProperties)
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
		for (vk::QueueFamilyProperties family : FamilyProperties)
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
		for (vk::QueueFamilyProperties family : FamilyProperties)
		{
			const bool presentationSupported = Device.getSurfaceSupportKHR(index, surface);
			if (family.queueFlags & vk::QueueFlagBits::eGraphics)
				indices.Graphics = index;

			if (family.queueCount > 0 && presentationSupported)
				indices.Present = index;

			if (indices.Graphics == indices.Present)
				break;

			index++;
		}

		index = 0;
		if (!indices.Transfer.has_value())
		{
			for (vk::QueueFamilyProperties family : FamilyProperties)
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
			for (vk::QueueFamilyProperties family : FamilyProperties)
			{
				if (family.queueFlags & vk::QueueFlagBits::eCompute)
				{
					indices.Compute = index;
					break;
				}
				index++;
			}
		}

		return indices;
	}

	uint32_t VulkanPhysicalDevice::GetSuitableMemoryTypeIndex(uint32_t memoryTypeBits, vk::MemoryPropertyFlags properties) const
	{
		for (uint32_t i = 0; i < MemoryProperties.memoryTypeCount; ++i)
		{
			if ((memoryTypeBits & (1 << i)) && (MemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		WI_CORE_ASSERT(false, "Faield to find suitable memory type");
		return -1;
	}
}
