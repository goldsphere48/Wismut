#if WI_PLATFORM_WIN
#include "WindowsMemory.h"

#include <Windows.h>

#include "Core/Assertion.h"

namespace Wi
{
	void* WindowsMemory::HeapAlloc(uint32 bufferSize)
	{
		HANDLE heap = GetProcessHeap();
		void* ptr = ::HeapAlloc(heap, HEAP_ZERO_MEMORY, bufferSize);
		CORE_CHECK(ptr)
		return ptr;
	}

	void WindowsMemory::HeapFree(void* buffer)
	{
		HANDLE heap = GetProcessHeap();
		::HeapFree(heap, 0, buffer);
	}
}
#endif
