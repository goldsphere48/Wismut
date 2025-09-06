#pragma once
#if WI_PLATFORM_WIN

#include "Core/CoreTypes.h"
#include "Platform/GenericPlatform/GenericPlatformMemory.h"

namespace Wi
{
	class WindowsMemory : public GenericPlatformMemory
	{
	public:
		static void* AlignedAlloc(uint64 size, uint64 alignment);
		static void  AlignedFree(void* ptr);
	};

	using PlatformMemory = WindowsMemory;
}
#endif
