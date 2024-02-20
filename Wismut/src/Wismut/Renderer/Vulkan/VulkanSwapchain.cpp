#include "wipch.h"
#include "VulkanSwapchain.h"

#include "VulkanUtils.h"
#include "Windows/WindowsVulkanPlatform.h"
#include "Wismut/Renderer/Renderer.h"

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
		vk::Extent2D extent = {
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

	void VulkanSwapchain::Present()
	{
		vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

		const vk::SubmitInfo submitInfo {
			.sType = vk::StructureType::eSubmitInfo,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_SyncHandlers[m_CurrentBufferIndex].AvailableSemaphore,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &m_CommandBuffers[m_CurrentBufferIndex],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &m_SyncHandlers[m_CurrentBufferIndex].RenderFinishedSemaphore
		};

		const vk::Result resetFencesResult = m_Device->LogicalDevice.resetFences(1, &m_SyncHandlers[m_CurrentBufferIndex].WaitFence);
		VK_CHECK_RESULT(resetFencesResult, "Failed to wait fences in Present");

		vk::Result result = m_Device->GetGraphicsQueue().submit(1, &submitInfo, m_SyncHandlers[m_CurrentBufferIndex].WaitFence);
		if (result != vk::Result::eSuccess)
			WI_CORE_ERROR("Submit failed");

		const vk::PresentInfoKHR presentInfo {
			.sType = vk::StructureType::ePresentInfoKHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_SyncHandlers[m_CurrentBufferIndex].RenderFinishedSemaphore,
			.swapchainCount = 1,
			.pSwapchains = &m_Swapchain,
			.pImageIndices = &m_CurrentImageIndex,
			.pResults = nullptr
		};

		result = m_Device->GetPresentQueue().presentKHR(&presentInfo);

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) 
		{
			WI_CORE_INFO("Swapchain is out of date and should be recreated");
		} else if (result != vk::Result::eSuccess) 
		{
			WI_CORE_ERROR("Present failed");
		}

		m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % Renderer::GetConfig().MaxFramesInFlight;

		const vk::Result waitForFencesResult = m_Device->LogicalDevice.waitForFences(1, &m_SyncHandlers[m_CurrentBufferIndex].WaitFence, VK_TRUE, UINT64_MAX);
		VK_CHECK_RESULT(waitForFencesResult, "Failed to wait fences in AcquireNextImage");
	}

	void VulkanSwapchain::BeginFrame()
	{
		m_CurrentImageIndex = AcquireNextImage();
		m_CommandBuffers[m_CurrentBufferIndex].reset();
	}

	uint32_t VulkanSwapchain::AcquireNextImage() const
	{
		const vk::Result waitForFencesResult = m_Device->LogicalDevice.waitForFences(1, &m_SyncHandlers[m_CurrentBufferIndex].WaitFence, VK_TRUE, UINT64_MAX);
		VK_CHECK_RESULT(waitForFencesResult, "Failed to wait fences in AcquireNextImage");

		const auto acquireResult = m_Device->LogicalDevice.acquireNextImageKHR(m_Swapchain, UINT64_MAX, m_SyncHandlers[m_CurrentBufferIndex].AvailableSemaphore);
		VK_CHECK_RESULT(acquireResult.result, "Faield when acquire next image");

		return acquireResult.value;
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
		Create();
	}

	void VulkanSwapchain::Create()
	{
		m_Device->LogicalDevice.waitIdle();

		const auto capabilities = GetCapabilities(m_Device->PhysicalDevice->Device);
		m_SurfaceFormat = ChooseFormat(capabilities.Formats);
		m_PresentMode = ChoosePresentMode(capabilities.PresentModes);
		m_SurfaceCapabilities = capabilities.SurfaceCapabilities;

		m_Extent = ChooseSwapExtent(m_SurfaceCapabilities);
		m_Width = m_Extent.width;
		m_Height = m_Extent.height;

		const auto imageCount = std::clamp(m_SurfaceCapabilities.minImageCount + 1, m_SurfaceCapabilities.minImageCount, m_SurfaceCapabilities.maxImageCount);

		const auto oldSwapchain = m_Swapchain;

		vk::SwapchainCreateInfoKHR swapchainCreateInfo {
			.sType = vk::StructureType::eSwapchainCreateInfoKHR,
			.surface = m_Surface,
			.minImageCount = imageCount,
			.imageFormat = m_SurfaceFormat.format,
			.imageColorSpace = m_SurfaceFormat.colorSpace,
			.imageExtent = m_Extent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.preTransform = m_SurfaceCapabilities.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode = m_PresentMode,
			.clipped = VK_TRUE,
			.oldSwapchain = oldSwapchain
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

		if (oldSwapchain)
			m_Device->LogicalDevice.destroySwapchainKHR(oldSwapchain);

		m_Device->LogicalDevice.waitIdle();

		const auto images = m_Device->LogicalDevice.getSwapchainImagesKHR(m_Swapchain);
		m_SwapchainResources.resize(images.size());

		for (uint32_t i = 0; i < images.size(); ++i)
		{
			vk::ImageViewCreateInfo imageViewCreateInfo {
				.sType = vk::StructureType::eImageViewCreateInfo,
				.image = images[i],
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
			m_SwapchainResources[i].Image = images[i];
			m_SwapchainResources[i].ImageView = view;
		}

		const vk::AttachmentDescription attachmentDescription {
			.format = m_SurfaceFormat.format,
			.loadOp = vk::AttachmentLoadOp::eClear,
			.storeOp = vk::AttachmentStoreOp::eStore,
			.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
			.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
			.initialLayout = vk::ImageLayout::eUndefined,
			.finalLayout = vk::ImageLayout::ePresentSrcKHR
		};

		vk::AttachmentReference colorAttachmentReference {
			.attachment = 0,
			.layout = vk::ImageLayout::eColorAttachmentOptimal
		};

		vk::SubpassDescription subpass {
			.colorAttachmentCount = 1,
			.pColorAttachments = &colorAttachmentReference
		};

		constexpr vk::SubpassDependency dependency {
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
			.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
			.srcAccessMask = vk::AccessFlagBits::eNone,
			.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
		};

		vk::RenderPassCreateInfo renderPassCreateInfo {
			.sType = vk::StructureType::eRenderPassCreateInfo,
			.attachmentCount = 1,
			.pAttachments = &attachmentDescription,
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &dependency
		};

		m_VkRenderPass = m_Device->LogicalDevice.createRenderPass(renderPassCreateInfo);

		for (auto& m_SwapchainResource : m_SwapchainResources)
		{
			const vk::FramebufferCreateInfo framebufferCreateInfo {
				.sType = vk::StructureType::eFramebufferCreateInfo,
				.renderPass = m_VkRenderPass,
				.attachmentCount = 1,
				.pAttachments = &m_SwapchainResource.ImageView,
				.width = m_Width,
				.height = m_Height,
				.layers = 1
			};

			const auto vkFramebuffer = m_Device->LogicalDevice.createFramebuffer(framebufferCreateInfo);
			m_SwapchainResource.Framebuffer = vkFramebuffer;
		}

		const vk::CommandPoolCreateInfo commandPoolCreateInfo {
			.sType = vk::StructureType::eCommandPoolCreateInfo,
			.flags = vk::CommandPoolCreateFlagBits::eTransient | vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			.queueFamilyIndex = m_Device->PhysicalDevice->QueueFamilyIndices.Graphics.value(),
		};

		m_VkCommandPool = m_Device->LogicalDevice.createCommandPool(commandPoolCreateInfo);

		const vk::CommandBufferAllocateInfo bufferAllocationInfo {
			.sType = vk::StructureType::eCommandBufferAllocateInfo,
			.commandPool = m_VkCommandPool,
			.level = vk::CommandBufferLevel::ePrimary,
			.commandBufferCount = static_cast<uint32_t>(m_SwapchainResources.size()),
		};

		m_CommandBuffers = m_Device->LogicalDevice.allocateCommandBuffers(bufferAllocationInfo);

		vk::SemaphoreCreateInfo semaphoreCreateInfo {
			.sType = vk::StructureType::eSemaphoreCreateInfo
		};

		vk::FenceCreateInfo fenceCreateInfo {
			.sType = vk::StructureType::eFenceCreateInfo,
			.flags = vk::FenceCreateFlagBits::eSignaled
		};

		uint32_t maxFramesInFlight = Renderer::GetConfig().MaxFramesInFlight;

		m_SyncHandlers.resize(maxFramesInFlight);

		for (uint32_t i = 0; i < maxFramesInFlight; ++i)
		{
			m_SyncHandlers[i].AvailableSemaphore = m_Device->LogicalDevice.createSemaphore(semaphoreCreateInfo);
			m_SyncHandlers[i].RenderFinishedSemaphore = m_Device->LogicalDevice.createSemaphore(semaphoreCreateInfo);
			m_SyncHandlers[i].WaitFence = m_Device->LogicalDevice.createFence(fenceCreateInfo);
		}

		m_Device->LogicalDevice.waitIdle();
	}

	void VulkanSwapchain::Destroy()
	{
		DestroySwapchain();
		m_Device->LogicalDevice.destroySwapchainKHR(m_Swapchain);
		m_Instance.destroySurfaceKHR(m_Surface);
	}

	void VulkanSwapchain::DestroySwapchain()
	{
		m_Device->LogicalDevice.waitIdle();

		for (int i = 0; i < Renderer::GetConfig().MaxFramesInFlight; ++i)
		{
			m_Device->LogicalDevice.destroySemaphore(m_SyncHandlers[i].RenderFinishedSemaphore);
			m_Device->LogicalDevice.destroySemaphore(m_SyncHandlers[i].AvailableSemaphore);
			m_Device->LogicalDevice.destroyFence(m_SyncHandlers[i].WaitFence);
		}

		m_SyncHandlers.clear();

		for (const auto& resource : m_SwapchainResources) 
		{
			m_Device->LogicalDevice.destroyImageView(resource.ImageView);
			m_Device->LogicalDevice.destroyFramebuffer(resource.Framebuffer);
		}
		m_SwapchainResources.clear();

		m_Device->LogicalDevice.freeCommandBuffers(m_VkCommandPool, m_CommandBuffers.size(), m_CommandBuffers.data());
		m_Device->LogicalDevice.destroyCommandPool(m_VkCommandPool);
		m_Device->LogicalDevice.destroyRenderPass(m_VkRenderPass);

		m_Device->LogicalDevice.waitIdle();
	}

	void VulkanSwapchain::RecreateSwapchain()
	{
		WI_CORE_INFO("Recreating swapchain...");
		if (m_Swapchain) {
			DestroySwapchain();
		}
		Create();
		WI_CORE_INFO("New size: {0} {1}", m_Width, m_Height);
		if (m_RecreateCallback) {
			m_RecreateCallback(m_Width, m_Height);
		}
	}
}
