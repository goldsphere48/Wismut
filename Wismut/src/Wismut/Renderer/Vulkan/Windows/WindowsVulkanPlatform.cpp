#include "wipch.h"

#ifdef WI_PLATFORM_WIN

#include "WindowsVulkanPlatform.h"

namespace Wi
{
	std::vector<const char*> WindowsVulkanPlatform::GetExtensions()
	{
		std::vector extensions = 
		{
			"VK_KHR_surface",
			"VK_KHR_win32_surface"
		};

		return extensions;
	}
}
#endif
