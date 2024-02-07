#include "wipch.h"
#include "VulkanSwapchain.h"

#include "Windows/WindowsVulkanPlatform.h"

namespace Wi
{
	static vk::SurfaceFormatKHR ChooseFormat(const std::vector<vk::SurfaceFormatKHR>& formats)
	{
		for (const auto format : formats)
			if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return format;
	}

	static vk::PresentModeKHR ChoosePresentMode(const std::vector<vk::PresentModeKHR>& modes)
	{
		for (const auto mode : modes)
			if (mode == vk::PresentModeKHR::eMailbox)
				return mode;

		return vk::PresentModeKHR::eFifo;
	}

	static vk::Extent2D ChooseSwapExtent(vk::SurfaceCapabilitiesKHR capabilities)
	{
		auto [width, height] = VulkanPlatform::GetFramebufferSize();
		vk::Extent2D extent =
		{
			width,
			height
		};

		extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return extent;
	}

	VulkanSwapchain::VulkanSwapchain(const vk::Instance& instance) : m_Instance(instance)
	{
		
	}

	void VulkanSwapchain::CreateSurface()
	{
		m_Surface = VulkanPlatform::CreateSurface(m_Instance);

		WI_CORE_ASSERT(m_Surface, "Failed to create vulkan surface");
	}

	VulkanSwapchainCapabilities VulkanSwapchain::GetCapabilities(const vk::PhysicalDevice& device) const
	{
		VulkanSwapchainCapabilities capabilities;
		capabilities.SurfaceCapabilities = device.getSurfaceCapabilitiesKHR(m_Surface);
		capabilities.Formats = device.getSurfaceFormatsKHR(m_Surface);
		capabilities.PresentModes = device.getSurfacePresentModesKHR(m_Surface);
		return capabilities;
	}

	void VulkanSwapchain::Initialize(const std::shared_ptr<VulkanDevice>& device)
	{
		m_Device = device;

		const auto capabilities = GetCapabilities(m_Device->PhysicalDevice->Device);
		m_SurfaceFormat = ChooseFormat(capabilities.Formats);
		m_PresentMode = ChoosePresentMode(capabilities.PresentModes);
		m_SurfaceCapabilities = capabilities.SurfaceCapabilities;

		Create();
	}

	void VulkanSwapchain::Create()
	{
		const auto extent = ChooseSwapExtent(m_SurfaceCapabilities);
		const auto imageCount = std::clamp(m_SurfaceCapabilities.minImageCount + 1, m_SurfaceCapabilities.minImageCount, m_SurfaceCapabilities.maxImageCount);

		vk::SwapchainCreateInfoKHR swapchainCreateInfo =
		{
			.sType = vk::StructureType::eSwapchainCreateInfoKHR,
			.surface = m_Surface,
			.minImageCount = imageCount,
			.imageFormat = m_SurfaceFormat.format,
			.imageColorSpace = m_SurfaceFormat.colorSpace,
			.imageExtent = extent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.preTransform = m_SurfaceCapabilities.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode = m_PresentMode,
			.clipped = VK_TRUE
		};

		const auto indices = m_Device->PhysicalDevice->QueueFamilyIndices;

		const uint32_t queueFamilyIndices[] = { indices.Graphics.value(), indices.Present.value() };

		if (indices.Graphics != indices.Present)
		{
			swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else
		{
			swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
			swapchainCreateInfo.queueFamilyIndexCount = 0;
			swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		}

		m_Swapchain = m_Device->LogicalDevice.createSwapchainKHR(swapchainCreateInfo);

		m_Device->LogicalDevice.waitIdle();

		m_Images = m_Device->LogicalDevice.getSwapchainImagesKHR(m_Swapchain);
		m_ImageViews.resize(m_Images.size());

		for (const auto& image : m_Images)
		{
			vk::ImageViewCreateInfo imageViewCreateInfo =
			{
				.sType = vk::StructureType::eImageViewCreateInfo,
				.image = image,
				.viewType = vk::ImageViewType::e2D,
				.format = m_SurfaceFormat.format,
				.components =
				{
					.r = vk::ComponentSwizzle::eIdentity,
					.g = vk::ComponentSwizzle::eIdentity,
					.b = vk::ComponentSwizzle::eIdentity,
					.a = vk::ComponentSwizzle::eIdentity,
				},
				.subresourceRange =
				{
					.aspectMask = vk::ImageAspectFlagBits::eColor,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			};

			const auto view = m_Device->LogicalDevice.createImageView(imageViewCreateInfo);
			m_ImageViews.push_back(view);
		}
	}

	void VulkanSwapchain::Destroy()
	{
		DestroySwapchain();
		m_Instance.destroySurfaceKHR(m_Surface);
	}

	void VulkanSwapchain::DestroySwapchain()
	{
		m_Device->LogicalDevice.waitIdle();

		for (const auto& view : m_ImageViews)
			m_Device->LogicalDevice.destroyImageView(view);

		m_ImageViews.clear();
		m_Images.clear();

		m_Device->LogicalDevice.destroySwapchainKHR(m_Swapchain);
	}

	void VulkanSwapchain::RecreateSwapchain()
	{
		DestroySwapchain();
		Create();
	}
}
