#pragma once
#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"
#include "VulkanRenderPass.h"

namespace Wi
{
	struct VulkanSwapchainCapabilities
	{
		vk::SurfaceCapabilitiesKHR SurfaceCapabilities;
		std::vector<vk::SurfaceFormatKHR> Formats;
		std::vector<vk::PresentModeKHR> PresentModes;

		bool IsSuitable() const
		{
			return Formats.size() > 0 && PresentModes.size() > 0;
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

		vk::SurfaceKHR GetSurface() const { return m_Surface; }
		vk::Format GetImageFormat() const { return m_SurfaceFormat.format; }
		VulkanSwapchainCapabilities GetCapabilities(const vk::PhysicalDevice& device) const;

	private:
		void Create();
		void DestroySwapchain();

	private:
		std::shared_ptr<VulkanDevice> m_Device;
		const vk::Instance& m_Instance;
		vk::SurfaceCapabilitiesKHR m_SurfaceCapabilities;
		vk::SurfaceKHR m_Surface;
		vk::SurfaceFormatKHR m_SurfaceFormat;
		vk::PresentModeKHR m_PresentMode;
		vk::SwapchainKHR m_Swapchain;
		vk::RenderPass m_VkRenderPass;
		vk::CommandPool m_VkCommandPool;
		
		std::vector<vk::ImageView> m_ImageViews;
		std::vector<vk::Image> m_Images;
		std::vector<vk::Framebuffer> m_Framebuffers;
		std::vector<vk::CommandBuffer> m_CommandBuffers;
	};
}
