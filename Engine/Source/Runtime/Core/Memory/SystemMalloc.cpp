#include "SystemMalloc.h"

#include "Platform/Platform.h"

namespace Wi
{
	void* SystemMalloc::Allocate(usize size, usize alignment)
	{
		std::lock_guard lock(m_Mutex);
		void* ptr = PlatformMemory::AlignedAlloc(size, alignment);
		if (ptr == nullptr && size)
		{
			// TODO: Handle OOM
			return nullptr;
		}
		return ptr;
	}

	void SystemMalloc::Free(void* ptr)
	{
		std::lock_guard lock(m_Mutex);
		PlatformMemory::AlignedFree(ptr);
	}
}
