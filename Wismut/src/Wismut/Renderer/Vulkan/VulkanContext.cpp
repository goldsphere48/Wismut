#include "wipch.h"
#include "VulkanContext.h"

#include "VulkanUtils.h"
#include "VulkanPlatform.h"
#include "VulkanShader.h"
#include "VulkanShaderCompiler.h"

namespace Wi
{

#ifdef WI_DEBUG
	bool VulkanContext::s_ValidationEnabled = true;
#else
	bool VulkanContext::s_ValidationEnabled = false;
#endif

	struct PhysicDeviceRequirements
	{
		std::vector<vk::PhysicalDeviceType> SuitableDeviceTypes;
		std::vector<std::string> RequiredExtensions;
	};

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback
	(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	)
	{
		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			WI_CORE_WARN(pCallbackData->pMessage);

		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			WI_CORE_ERROR(pCallbackData->pMessage);

		return VK_FALSE;
	}

	static VulkanPhysicalDevice* SelectDevice(vk::Instance instance, const std::shared_ptr<VulkanSwapchain>& swapchain)
	{
		const auto deviceRequirements = PhysicDeviceRequirements
		{
			.SuitableDeviceTypes = { vk::PhysicalDeviceType::eDiscreteGpu, vk::PhysicalDeviceType::eIntegratedGpu },
			.RequiredExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME },
		};

		for (const auto requiredDeviceType : deviceRequirements.SuitableDeviceTypes)
		{
			for (const auto vkDevice : instance.enumeratePhysicalDevices())
			{
				const auto device = new VulkanPhysicalDevice(vkDevice);
				device->QueueFamilyIndices = device->GetQueueFamilies(swapchain->GetSurface());

				if (!device->QueueFamilyIndices.IsComplete())
					continue;

				bool allExtensionsSupported = true;
				if (device->Properties.deviceType == requiredDeviceType)
				{
					for (const auto& requiredExtension : deviceRequirements.RequiredExtensions)
					{
						const auto found = device->IsSupportExtension(requiredExtension);

						if (!found)
							allExtensionsSupported = false;
					}
				}

				if (!swapchain->GetCapabilities(vkDevice).IsSuitable())
					continue;

				if (allExtensionsSupported)
					return device;
			}
		}

		return nullptr;
	}

	void VulkanContext::Initialize()
	{
		constexpr vk::ApplicationInfo appInfo = 
		{
			.sType = vk::StructureType::eApplicationInfo,
			.pApplicationName = "Wismut App",
			.applicationVersion = VK_MAKE_VERSION(0, 0, 1),
			.pEngineName = "Wismut",
			.engineVersion = VK_MAKE_VERSION(0, 0, 1),
			.apiVersion = VK_API_VERSION_1_3,
		};

		auto extensions = VulkanPlatform::GetExtensions();

		const vk::DebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{
			.sType = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT,
			.messageSeverity =
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			.messageType =
				vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
			.pfnUserCallback = DebugMessengerCallback,
			.pUserData = nullptr
		};

		std::vector<const char*> validationLayers;

		if (s_ValidationEnabled) 
		{
			extensions.push_back("VK_EXT_debug_utils");
			validationLayers.push_back("VK_LAYER_KHRONOS_validation");
		}

		const vk::InstanceCreateInfo instanceCreateInfo = 
		{
			.sType = vk::StructureType::eInstanceCreateInfo,
			.pNext = &debugMessengerCreateInfo,
			.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()),
			.ppEnabledLayerNames = validationLayers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data(),
		};

		m_VkInstance = vk::createInstance(instanceCreateInfo);
		WI_CORE_ASSERT(m_VkInstance, "Failed to create vulkan instance")

		m_DynamicLoader = vk::DispatchLoaderDynamic(m_VkInstance, vkGetInstanceProcAddr);

		if (s_ValidationEnabled) 
		{
			const auto result = m_VkInstance.createDebugUtilsMessengerEXT(&debugMessengerCreateInfo, nullptr, &m_DebugMessenger, m_DynamicLoader);
			VK_CHECK_RESULT(result, "Failed to create debug messenger")
		}

		m_Swapchain = std::make_shared<VulkanSwapchain>(m_VkInstance);
		m_Swapchain->CreateSurface();

		m_PhysicalDevice = SelectDevice(m_VkInstance, m_Swapchain);
		m_Device = std::make_shared<VulkanDevice>(m_VkInstance, m_PhysicalDevice);

		m_Swapchain->Initialize(m_Device);
	}

	void VulkanContext::Destroy()
	{
		m_Swapchain->Destroy();
		m_Device->Destroy();
		m_VkInstance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DynamicLoader);
		m_VkInstance.destroy();
		delete m_PhysicalDevice;
	}
}
