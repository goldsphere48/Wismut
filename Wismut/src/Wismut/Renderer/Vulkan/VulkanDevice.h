#pragma once

#include <vulkan/vulkan.hpp>

#include "VulkanPhysicalDevice.h"

namespace Wi
{
	class VulkanDevice
	{
	public:
		VulkanDevice(vk::Instance instance, VulkanPhysicalDevice* physicalDevice);

		void Destroy() const;

		const vk::Device& GetLogicalDevice() const { return m_LogicalDevice; }
		const vk::Queue& GetGraphicsQueue() const { return m_GraphicsQueue; }
		const vk::Queue& GetComputeQueue() const { return m_ComputeQueue; }
		const vk::Queue& GetTransferQueue() const { return m_TransferQueue; }

	private:
		vk::Device m_LogicalDevice;
		vk::Queue m_GraphicsQueue;
		vk::Queue m_TransferQueue;
		vk::Queue m_ComputeQueue;
	};
}

