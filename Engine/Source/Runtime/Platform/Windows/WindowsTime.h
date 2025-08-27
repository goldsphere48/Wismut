#pragma once
#if WI_PLATFORM_WIN

#include "Platform/GenericPlatform/GenericPlatformTime.h"

namespace Wi
{
	class WindowsTime : public GenericPlatformTime
	{
	public:
		static DateTime GetLocalTime();
	};

	using PlatformTime = WindowsTime;
}
#endif
