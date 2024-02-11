#pragma once

#include "VulkanRendererAPI.h"
#include "Wismut/Renderer/RenderPass.h"

namespace Wi
{
	struct VulkanRenderPass : RenderPass
	{
		vk::RenderPass VkRenderPass;
	};
}
