#pragma once
#include "Core/CoreTypes.h"

namespace Wi
{
	class DefaultAllocator
	{
	public:
		void* Allocate(uint64 size, uint64 alignment = 0);
		void* Reallocate(void* ptr, uint64 newSize, uint64 alignment = 0);
		void Free(void* ptr);
	};
}
