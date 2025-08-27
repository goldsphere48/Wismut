#pragma once
#if WI_PLATFORM_WIN

#include "Platform/GenericPlatform/GenericPlatformMemory.h"

namespace Wi
{
	class WindowsMemory : public GenericPlatformMemory
	{
	public:
		static void* HeapAlloc(uint32 bufferSize);

		static void HeapFree(void* buffer);
	};

	using PlatformMemory = WindowsMemory;
}
#endif
