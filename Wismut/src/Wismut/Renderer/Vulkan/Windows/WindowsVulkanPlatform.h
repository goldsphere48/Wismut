#pragma once

#ifdef WI_PLATFORM_WIN

#include <vulkan/vulkan.hpp>

namespace Wi
{
	class WindowsVulkanPlatform
	{
	public:
		static std::vector<const char*> GetExtensions();
		static vk::SurfaceKHR CreateSurface(const vk::Instance& instance);
	};

	using VulkanPlatform = WindowsVulkanPlatform;
}
#endif
