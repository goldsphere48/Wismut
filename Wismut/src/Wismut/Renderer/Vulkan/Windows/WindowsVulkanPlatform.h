#pragma once

#ifdef WI_PLATFORM_WIN
namespace Wi
{
	class WindowsVulkanPlatform
	{
	public:
		static std::vector<const char*> GetExtensions();
	};

	using VulkanPlatform = WindowsVulkanPlatform;
}
#endif
