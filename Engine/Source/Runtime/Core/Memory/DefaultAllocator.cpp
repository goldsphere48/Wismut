#include "DefaultAllocator.h"

#include "Memory.h"

namespace Wi
{
	void* DefaultAllocator::Allocate(uint64 size, uint64 alignment)
	{
		return WI_ALLOC_ALIGNED(size, alignment);
	}

	void* DefaultAllocator::Reallocate(void* ptr, uint64 newSize, uint64 alignment)
	{
		return WI_REALLOC_ALIGNED(ptr, newSize, alignment);
	}

	void DefaultAllocator::Free(void* ptr)
	{
		WI_FREE(ptr);
	}
}
