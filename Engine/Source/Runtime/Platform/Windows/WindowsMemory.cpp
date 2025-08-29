#if WI_PLATFORM_WIN
#include "WindowsMemory.h"

#include "Core/Assertion.h"
#include "Core/Math/MathUtils.h"

namespace Wi
{
	void* WindowsMemory::AlignedAlloc(usize size, usize alignment)
	{
		alignment = Math::Max(alignment, size >= DefaultAlignment ? DefaultAlignment : 8);
		void* ptr = _aligned_malloc(size, alignment);
		return ptr;
	}

	void WindowsMemory::AlignedFree(void* ptr)
	{
		_aligned_free(ptr);
	}
}
#endif
