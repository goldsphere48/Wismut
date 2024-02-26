#pragma once
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"

namespace Wi
{
	struct VulkanSwapchainCapabilities
	{
		vk::SurfaceCapabilitiesKHR SurfaceCapabilities;
		std::vector<vk::SurfaceFormatKHR> Formats;
		std::vector<vk::PresentModeKHR> PresentModes;

		bool IsSuitable() const
		{
			return !Formats.empty() && !PresentModes.empty();
		}
	};

	class VulkanSwapchain
	{
		using RecreateCallback = std::function<void(uint32_t, uint32_t)>;

		struct VulkanSyncHandlers
		{
			vk::Semaphore AvailableSemaphore;
			vk::Semaphore RenderFinishedSemaphore;
			vk::Fence WaitFence;
		};

		struct VulkanSwapchainResource
		{
			vk::ImageView ImageView;
			vk::Image Image;
			vk::Framebuffer Framebuffer;
		};

	public:
		VulkanSwapchain(const vk::Instance& instance);

		void Initialize(const std::shared_ptr<VulkanDevice>& device);
		void SetRecreateCallback(const RecreateCallback& callback) { m_RecreateCallback = callback; }

		void Destroy();
		void RecreateSwapchain();
		void CreateSurface();
		void Present();
		void BeginFrame();

		uint32_t GetWidth() const { return m_Extent.width; }
		uint32_t GetHeight() const { return m_Extent.height; }
		vk::SurfaceKHR GetSurface() const { return m_Surface; }
		vk::Format GetImageFormat() const { return m_SurfaceFormat.format; }
		vk::Extent2D GetExtent() const { return m_Extent; }
		const vk::RenderPass& GetVkRenderPass() const { return m_VkRenderPass; }
		VulkanSwapchainCapabilities GetCapabilities(const vk::PhysicalDevice& device) const;
		const vk::Framebuffer& GetCurrentFramebuffer() const { return m_SwapchainResources[m_CurrentImageIndex].Framebuffer; }
		const vk::CommandBuffer& GetCurrentCommandBuffer() const { return m_CommandBuffers[m_CurrentBufferIndex]; }
		uint32_t GetImagesCount() const { return static_cast<uint32_t>(m_SwapchainResources.size()); }

	private:
		void Create();
		void DestroySwapchain();
		uint32_t AcquireNextImage() const;

	private:
		uint32_t m_CurrentImageIndex = 0;
		uint32_t m_CurrentBufferIndex = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		RecreateCallback m_RecreateCallback = nullptr;

		std::shared_ptr<VulkanDevice> m_Device;
		const vk::Instance& m_Instance;
		vk::SurfaceCapabilitiesKHR m_SurfaceCapabilities;
		vk::SurfaceKHR m_Surface;
		vk::SurfaceFormatKHR m_SurfaceFormat;
		vk::PresentModeKHR m_PresentMode;
		vk::Extent2D m_Extent;
		vk::SwapchainKHR m_Swapchain;
		vk::RenderPass m_VkRenderPass;
		
		std::vector<VulkanSwapchainResource> m_SwapchainResources;
		std::vector<VulkanSyncHandlers> m_SyncHandlers;
		std::vector<vk::CommandBuffer> m_CommandBuffers;
	};
}
