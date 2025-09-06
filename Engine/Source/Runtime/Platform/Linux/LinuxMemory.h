#pragma once
#ifdef WI_PLATFORM_LINUX

#include "Core/CoreTypes.h"
#include "Platform/GenericPlatform/GenericPlatformMemory.h"

namespace Wi
{
	class LinuxMemory : public GenericPlatformMemory
	{
	public:
		static void* AlignedAlloc(uint64 size, uint64 alignment);
		static void  AlignedFree(void* ptr);
	};

	using PlatformMemory = LinuxMemory;
}
#endif
