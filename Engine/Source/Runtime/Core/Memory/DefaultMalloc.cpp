#include "DefaultMalloc.h"

#include "Platform/Platform.h"

namespace Wi
{
	void* DefaultMalloc::Allocate(uint64 size, uint64 alignment)
	{
		return PlatformMemory::AlignedAlloc(size, alignment);
	};

	void DefaultMalloc::Free(void* ptr)
	{
		PlatformMemory::AlignedFree(ptr);
	};
}
