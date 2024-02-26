#pragma once

#include <vulkan/vulkan.hpp>

#include "VulkanPhysicalDevice.h"

namespace Wi
{
	class VulkanDevice
	{
	public:
		VulkanDevice(vk::Instance instance, std::shared_ptr<VulkanPhysicalDevice> physicalDevice);

		void Destroy() const;

		const vk::Queue& GetGraphicsQueue() const { return m_GraphicsQueue; }
		const vk::Queue& GetComputeQueue() const { return m_ComputeQueue; }
		const vk::Queue& GetTransferQueue() const { return m_TransferQueue; }
		const vk::Queue& GetPresentQueue() const { return m_PresentQueue; }

		std::shared_ptr<VulkanPhysicalDevice> PhysicalDevice;
		vk::Device LogicalDevice;
		vk::CommandPool CommandPool;

	private:
		vk::Queue m_GraphicsQueue;
		vk::Queue m_TransferQueue;
		vk::Queue m_ComputeQueue;
		vk::Queue m_PresentQueue;
	};
}

