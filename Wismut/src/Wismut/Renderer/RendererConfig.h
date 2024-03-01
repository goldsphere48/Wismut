#pragma once
#include "RendererAPI.h"

namespace Wi
{
	struct RendererConfig
	{
		RendererAPIType RenderAPIType = RendererAPIType::Vulkan;
		uint32_t MaxFramesInFlight = 3;
	};
}
