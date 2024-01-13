#include "wipch.h"
#include "VulkanDevice.h"

namespace Wi
{
	struct PhysicDeviceRequirements
	{
		std::vector<vk::PhysicalDeviceType> SuitableDeviceTypes;
		std::vector<std::string> RequiredExtensions;
	};

	VulkanQueueFamilyIndices FindQueueFamilyIndices(const vk::PhysicalDevice& device)
	{
		VulkanQueueFamilyIndices indices;

		const auto queueFamilies = device.getQueueFamilyProperties();

		uint32_t index = 0;
		for (auto family : queueFamilies)
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
		for (auto family : queueFamilies)
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
		for (auto family : queueFamilies)
		{
			if (family.queueFlags & vk::QueueFlagBits::eGraphics	)
			{
				indices.Graphics = index;
				break;
			}
			index++;
		}

		index = 0;
		if (!indices.Transfer.has_value())
		{
			for (auto family : queueFamilies)
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
			for (auto family : queueFamilies)
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
			for (auto family : queueFamilies)
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

	vk::PhysicalDevice SelectGPU(vk::Instance instance)
	{
		const auto deviceRequirements = PhysicDeviceRequirements
		{
			.SuitableDeviceTypes = { vk::PhysicalDeviceType::eDiscreteGpu, vk::PhysicalDeviceType::eIntegratedGpu },
			.RequiredExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME },
		};

		for (const auto requiredDeviceType : deviceRequirements.SuitableDeviceTypes)
		{
			for (const auto device : instance.enumeratePhysicalDevices())
			{
				if (!FindQueueFamilyIndices(device).IsComplete())
					continue;

				const auto deviceProperties = device.getProperties();
				bool allExtensionsSupported = true;
				if (deviceProperties.deviceType == requiredDeviceType)
				{
					const auto extensions = device.enumerateDeviceExtensionProperties();
					for (const auto& requiredExtension : deviceRequirements.RequiredExtensions)
					{
						auto found = std::ranges::find_if(extensions, [requiredExtension](const vk::ExtensionProperties prop)
							{
								return prop.extensionName.data() == requiredExtension;
							});

						if (found == extensions.end())
							allExtensionsSupported = false;
					}
				}

				if (allExtensionsSupported)
					return device;
			}
		}

		return nullptr;
	}

	VulkanDevice::VulkanDevice(vk::Instance instance)
	{
		m_vkPhysicalDevice = SelectGPU(instance);
		m_QueueFamilyIndices = FindQueueFamilyIndices(m_vkPhysicalDevice);
	}

	VulkanDevice::~VulkanDevice()
	{

	}
}
