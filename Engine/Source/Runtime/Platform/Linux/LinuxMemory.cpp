#ifdef WI_PLATFORM_LINUX
#include "LinuxMemory.h"

#include <cstdlib>

#include "Core/Math/MathUtils.h"

namespace Wi
{
	void* LinuxMemory::AlignedAlloc(usize size, usize alignment)
	{
		alignment = Math::Max(alignment, size >= DefaultAlignment ? DefaultAlignment : 8);
		uint32 alignedSize = (size + alignment - 1) & ~(alignment - 1);
		void* ptr = aligned_alloc(alignment, alignedSize);
		return ptr;
	}

	void LinuxMemory::AlignedFree(void* ptr)
	{
		free(ptr);
	}
}
#endif

