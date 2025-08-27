#pragma once
#ifdef WI_PLATFORM_LINUX

#include "Platform/GenericPlatform/GenericPlatformMemory.h"

namespace Wi
{
	class LinuxMemory : public GenericPlatformMemory
	{
	
	};

	using PlatformMemory = LinuxMemory;
}
#endif
