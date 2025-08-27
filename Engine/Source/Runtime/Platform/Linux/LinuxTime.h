#pragma once
#ifdef WI_PLATFORM_LINUX

#include "Platform/GenericPlatform/GenericPlatformTime.h"

namespace Wi
{
	class LinuxTime : public GenericPlatformTime
	{
	public:
		static DateTime GetLocalTime();
	};

	using PlatformTime = LinuxTime;
}
#endif
