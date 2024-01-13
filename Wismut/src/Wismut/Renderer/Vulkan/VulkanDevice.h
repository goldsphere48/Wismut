#pragma once

#include <vulkan/vulkan.hpp>

namespace Wi
{
	struct VulkanQueueFamilyIndices
	{
		std::optional<uint32_t> Graphics;
		std::optional<uint32_t> Transfer;
		std::optional<uint32_t> Compute;

		bool IsComplete() const
		{
			return Graphics.has_value() && Transfer.has_value() && Compute.has_value();
		}
	};

	class VulkanDevice
	{
	public:
		VulkanDevice(vk::Instance instance);

		void Destroy() const;

		const vk::PhysicalDevice& GetPhysicalDevice() const { return m_PhysicalDevice; }
		const vk::Device& GetLogicalDevice() const { return m_LogicalDevice; }
		const VulkanQueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }
		const vk::Queue& GetGraphicsQueue() const { return m_GraphicsQueue; }
		const vk::Queue& GetComputeQueue() const { return m_ComputeQueue; }
		const vk::Queue& GetTransferQueue() const { return m_TransferQueue; }

	private:
		vk::PhysicalDevice m_PhysicalDevice;
		vk::Device m_LogicalDevice;
		vk::Queue m_GraphicsQueue;
		vk::Queue m_TransferQueue;
		vk::Queue m_ComputeQueue;
		VulkanQueueFamilyIndices m_QueueFamilyIndices;
	};
}

