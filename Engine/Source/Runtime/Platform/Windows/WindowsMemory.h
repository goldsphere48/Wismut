#pragma once
#if WI_PLATFORM_WIN

#include "Platform/GenericPlatform/GenericPlatformMemory.h"

namespace Wi
{
	class WindowsMemory : public GenericPlatformMemory
	{
	public:
		static void* AlignedAlloc(usize size, usize alignment);
		static void AlignedFree(void* ptr);
	};

	using PlatformMemory = WindowsMemory;
}
#endif
