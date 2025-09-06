#ifdef WI_PLATFORM_LINUX
#include "LinuxMemory.h"
#endif
#include <cstdlib>

#include "Core/Assertion.h"
#include "Core/Templates/AlignTemplates.h"

namespace Wi
{
	void* LinuxMemory::AlignedAlloc(uint64 size, uint64 alignment)
	{
		CORE_CHECK(size == Align(size, alignment))
		return aligned_alloc(alignment, size);
	}

	void LinuxMemory::AlignedFree(void* ptr)
	{
		free(ptr);
	}
}
