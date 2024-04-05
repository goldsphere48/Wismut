#include "wipch.h"
#include "VulkanDescriptorSetManager.h"

#include "VulkanUtils.h"
#include "Wismut/Renderer/RendererCommon.h"

namespace Wi
{
	VulkanDescriptorSetManager::VulkanDescriptorSetManager(vk::Device device) : m_Device(device)
	{
		std::unordered_map<DescriptorType, uint32_t> poolSizesPerType
		{
			{DescriptorType::UniformBuffer, 1024}
		};

		std::vector<vk::DescriptorPoolSize> vkPoolSizes;

		for (auto [type, value] : poolSizesPerType)
		{
			vkPoolSizes.emplace_back(VulkanUtils::ConvertToVkDescriptorType(type), value);
		}

		const vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo
		{
			.sType = vk::StructureType::eDescriptorPoolCreateInfo,
			.maxSets = 1024,
			.poolSizeCount = vkPoolSizes.size(),
			.pPoolSizes = vkPoolSizes.data()
		};

		m_DescriptorPool = m_Device.createDescriptorPool(descriptorPoolCreateInfo);
	}

	void VulkanDescriptorSetManager::Destroy() const
	{
		m_Device.destroyDescriptorPool(m_DescriptorPool);
	}
}
