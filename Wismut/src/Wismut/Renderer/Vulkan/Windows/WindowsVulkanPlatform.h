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
		static std::pair<uint32_t, uint32_t> GetFramebufferSize();
	};

	using VulkanPlatform = WindowsVulkanPlatform;
}
#endif
