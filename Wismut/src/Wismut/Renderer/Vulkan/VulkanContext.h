#pragma once
#include "Wismut/Renderer/RendererContext.h"
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanRendererAPI.h"
#include "VulkanSwapchain.h"

namespace Wi
{
	class VulkanContext : public RendererContext
	{
	public:
		void Initialize() override;
		void Destroy() override;

		void OnWindowResize(int width, int height) override;

		const std::shared_ptr<VulkanDevice>& GetDevice() const { return m_Device; }
		const std::shared_ptr<VulkanSwapchain>& GetSwapchain() const { return m_Swapchain; }
		RendererAPI* GetApi() const override { return m_RendererApi; }
		const vk::Framebuffer& GetCurrentFrameBuffer() const { return m_Swapchain->GetCurrentFramebuffer(); }
		const vk::CommandBuffer& GetCurrentCommandBuffer() const { return m_Swapchain->GetCurrentCommandBuffer(); }

	private:
		static bool s_ValidationEnabled;

		VulkanRendererAPI* m_RendererApi;
		vk::Instance m_VkInstance;
		vk::DispatchLoaderDynamic m_DynamicLoader;
		vk::DebugUtilsMessengerEXT m_DebugMessenger;
		std::shared_ptr<VulkanPhysicalDevice> m_PhysicalDevice;
		std::shared_ptr<VulkanDevice> m_Device;
		std::shared_ptr<VulkanSwapchain> m_Swapchain;
	};
}
