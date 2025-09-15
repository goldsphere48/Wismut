#ifdef WI_PLATFORM_LINUX
#include "LinuxMemory.h"
#endif
#include <cstdlib>
#include <malloc.h>

#include "Core/Assertion.h"
#include "Core/Templates/AlignTemplates.h"

namespace Wi
{
	void* LinuxMemory::AlignedAlloc(uint64 size, uint64 alignment)
	{
		CORE_CHECK(size == Align(size, alignment))
		return aligned_alloc(alignment, size);
	}

	void* LinuxMemory::AlignedRealloc(void* ptr, uint64 newSize, uint64 alignment)
	{
		void* result = nullptr;
		if (ptr && newSize)
		{
			usize usableSize = malloc_usable_size(ptr);
			if (posix_memalign(&result, alignment, newSize) != 0)
			{
				result = nullptr;
			}
			else if (usableSize != 0)
			{
				MemCopy(result, ptr, usableSize);
			}
			free(ptr);
		}
		else if (ptr == nullptr)
		{
			if (posix_memalign(&result, alignment, newSize) != 0)
			{
				result = nullptr;
			}
		}
		else
		{
			free(ptr);
			result = nullptr;
		}
		return result;
	}

	void LinuxMemory::AlignedFree(void* ptr)
	{
		free(ptr);
	}
}
