#if WI_PLATFORM_WIN
#include "WindowsMemory.h"

#include "Core/Assertion.h"

namespace Wi
{
	void* WindowsMemory::AlignedAlloc(uint64 size, uint64 alignment)
	{
		return _aligned_malloc(size, alignment);
	}

	void WindowsMemory::AlignedFree(void* ptr)
	{
		_aligned_free(ptr);
	}
}
#endif
