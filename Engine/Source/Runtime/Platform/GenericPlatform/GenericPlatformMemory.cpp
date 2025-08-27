#include "GenericPlatformMemory.h"

#include <memory>

#include "Core/Assertion.h"
#include "Core/CoreTypes.h"

namespace Wi
{
	void* GenericPlatformMemory::HeapAlloc(uint32 bufferSize)
	{
		void* ptr = malloc(bufferSize);
		CORE_CHECK(ptr);
		return ptr;
	}

	void GenericPlatformMemory::HeapFree(void* buffer)
	{
		free(buffer);
	}
}
