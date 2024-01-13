#pragma once
#include "Wismut/Renderer/RendererContext.h"
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"

namespace Wi
{
	class VulkanContext : public RendererContext
	{
	public:
		virtual ~VulkanContext() = default;

		void Initialize() override;
		void Destroy() override;

	private:
		static bool s_ValidationEnabled;

		vk::Instance m_VkInstance;
		vk::DispatchLoaderDynamic m_DynamicLoader;
		vk::DebugUtilsMessengerEXT m_DebugMessenger;
		std::unique_ptr<VulkanDevice> m_Device;
	};
}
