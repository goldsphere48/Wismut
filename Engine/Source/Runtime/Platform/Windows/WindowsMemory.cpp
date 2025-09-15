#if WI_PLATFORM_WIN
#include "WindowsMemory.h"

#include "Core/Assertion.h"

namespace Wi
{
	void* WindowsMemory::AlignedAlloc(uint64 size, uint64 alignment)
	{
		return _aligned_malloc(size, alignment);
	}

	void* WindowsMemory::AlignedRealloc(void* ptr, uint64 newSize, uint64 alignment)
	{
		if (ptr && newSize)
		{
			return _aligned_realloc(ptr, newSize, alignment);
		}

		if (ptr == nullptr)
		{
			return _aligned_malloc(newSize, alignment);
		}

		_aligned_free(ptr);
		return nullptr;
	}

	void WindowsMemory::AlignedFree(void* ptr)
	{
		_aligned_free(ptr);
	}
}
#endif
