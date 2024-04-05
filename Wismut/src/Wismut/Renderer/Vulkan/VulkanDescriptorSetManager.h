#pragma once
#include "VulkanDevice.h"

namespace Wi
{
	class VulkanDescriptorSetManager
	{
	public:
		VulkanDescriptorSetManager(vk::Device device);
		const vk::DescriptorPool& GetPool() const { return m_DescriptorPool; }
		void Destroy() const;

	private:
		vk::Device m_Device;
		vk::DescriptorPool m_DescriptorPool;
	};
}
