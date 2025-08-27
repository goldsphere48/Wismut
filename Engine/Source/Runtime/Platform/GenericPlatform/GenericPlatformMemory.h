#pragma once
#include "Core/CoreTypes.h"

namespace Wi
{
	class GenericPlatformMemory
	{
	public:

		static void* HeapAlloc(uint32 bufferSize);

		static void HeapFree(void* buffer);
	};
}
