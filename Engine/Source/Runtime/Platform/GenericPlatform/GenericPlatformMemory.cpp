#include "GenericPlatformMemory.h"

#include <cstring>

namespace Wi
{
	void GenericPlatformMemory::MemCopy(void* dst, const void* src, usize size)
	{
		memcpy(dst, src, size);
	}
}
