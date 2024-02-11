#include "wipch.h"
#include "VulkanDevice.h"

#include "VulkanUtils.h"

namespace Wi
{
	VulkanDevice::VulkanDevice(vk::Instance instance, std::shared_ptr<VulkanPhysicalDevice> physicalDevice) : PhysicalDevice(physicalDevice)
	{
		constexpr float queuePriority = 1.0f;

		const vk::DeviceQueueCreateInfo graphicsQueueCreateInfo =
		{
			.sType = vk::StructureType::eDeviceQueueCreateInfo,
			.queueFamilyIndex = physicalDevice->QueueFamilyIndices.Graphics.value(),
			.queueCount = 1,
			.pQueuePriorities = &queuePriority,
		};

		const vk::DeviceQueueCreateInfo transferQueueCreateInfo =
		{
			.sType = vk::StructureType::eDeviceQueueCreateInfo,
			.queueFamilyIndex = physicalDevice->QueueFamilyIndices.Transfer.value(),
			.queueCount = 1,
			.pQueuePriorities = &queuePriority,
		};

		const vk::DeviceQueueCreateInfo computeQueueCreateInfo =
		{
			.sType = vk::StructureType::eDeviceQueueCreateInfo,
			.queueFamilyIndex = physicalDevice->QueueFamilyIndices.Compute.value(),
			.queueCount = 1,
			.pQueuePriorities = &queuePriority,
		};

		std::vector queueCreateInfos =
		{
			graphicsQueueCreateInfo,
			transferQueueCreateInfo,
			computeQueueCreateInfo
		};

		vk::DeviceQueueCreateInfo presentQueueCreateInfo;
		ZeroVulkanStruct<vk::DeviceQueueCreateInfo>(presentQueueCreateInfo, vk::StructureType::eDeviceQueueCreateInfo);

		if (physicalDevice->QueueFamilyIndices.Graphics != physicalDevice->QueueFamilyIndices.Present)
		{
			presentQueueCreateInfo.pQueuePriorities = &queuePriority;
			presentQueueCreateInfo.queueCount = 1;
			presentQueueCreateInfo.queueFamilyIndex = physicalDevice->QueueFamilyIndices.Present.value();

			queueCreateInfos.push_back(presentQueueCreateInfo);
		}

		const std::vector extensions = 
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		constexpr vk::PhysicalDeviceFeatures deviceFeatures
		{
			.geometryShader = true,
			.wideLines = true
		};

		const vk::DeviceCreateInfo deviceCreateInfo =
		{
			.sType = vk::StructureType::eDeviceCreateInfo,
			.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
			.pQueueCreateInfos = queueCreateInfos.data(),
			.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data(),
			.pEnabledFeatures = &deviceFeatures,
		};

		LogicalDevice = physicalDevice->Device.createDevice(deviceCreateInfo);

		m_GraphicsQueue = LogicalDevice.getQueue(physicalDevice->QueueFamilyIndices.Graphics.value(), 0);
		m_TransferQueue = LogicalDevice.getQueue(physicalDevice->QueueFamilyIndices.Transfer.value(), 0);
		m_ComputeQueue = LogicalDevice.getQueue(physicalDevice->QueueFamilyIndices.Compute.value(), 0);
		m_PresentQueue = LogicalDevice.getQueue(physicalDevice->QueueFamilyIndices.Present.value(), 0);
	}

	void VulkanDevice::Destroy() const
	{
		LogicalDevice.destroy();
	}
}
