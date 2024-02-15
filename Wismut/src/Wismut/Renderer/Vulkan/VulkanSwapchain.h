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
	public:
		VulkanSwapchain(const vk::Instance& instance);

		void Initialize(const std::shared_ptr<VulkanDevice>& device);

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
		const vk::Framebuffer& GetCurrentFramebuffer() const { return m_Framebuffers[m_CurrentImageIndex]; }
		const vk::CommandBuffer& GetCurrentCommandBuffer() const { return m_CommandBuffers[m_CurrentImageIndex]; }

	private:
		void Create();
		void DestroySwapchain();
		uint32_t AcquireNextImage() const;

	private:
		uint32_t m_CurrentImageIndex = 0;

		std::shared_ptr<VulkanDevice> m_Device;
		const vk::Instance& m_Instance;
		vk::SurfaceCapabilitiesKHR m_SurfaceCapabilities;
		vk::SurfaceKHR m_Surface;
		vk::SurfaceFormatKHR m_SurfaceFormat;
		vk::PresentModeKHR m_PresentMode;
		vk::Extent2D m_Extent;
		vk::SwapchainKHR m_Swapchain;
		vk::RenderPass m_VkRenderPass;
		vk::CommandPool m_VkCommandPool;
		
		std::vector<vk::ImageView> m_ImageViews;
		std::vector<vk::Image> m_Images;
		std::vector<vk::Framebuffer> m_Framebuffers;
		std::vector<vk::CommandBuffer> m_CommandBuffers;
		std::vector<vk::Semaphore> m_ImagesAvailableSemaphores;
		std::vector<vk::Semaphore> m_RenderFinishedSemaphores;
		std::vector<vk::Fence> m_InFlightFences;
	};
}
