#pragma once
#include "Core/CoreTypes.h"

namespace Wi
{
	class GenericPlatformMemory
	{
	public:
		static void MemCopy(void* dst, const void* src, usize size);
	};
}
