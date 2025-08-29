#pragma once
#ifdef WI_PLATFORM_LINUX

#include "Platform/GenericPlatform/GenericPlatformMemory.h"

namespace Wi
{
	class LinuxMemory : public GenericPlatformMemory
	{
	public:
		static void* AlignedAlloc(usize size, usize alignment);
		static void AlignedFree(void* ptr);
	};

	using PlatformMemory = LinuxMemory;
}
#endif
