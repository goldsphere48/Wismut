#ifdef WI_PLATFORM_WIN
#include "Core/Memory/SystemAllocator.h"

#include <Windows.h>

#include "Core/Assertion.h"
#include "Core/Math/Math.h"

namespace Wi
{
	void* SystemAllocator::Allocate(usize size, usize alignment)
	{
		WI_ASSERT(Math::IsPowOf2(alignment))

		return _aligned_malloc(size, alignment);
	}

	void* SystemAllocator::Reallocate(void* ptr, usize oldSize, usize newSize, usize alignment)
	{
		return _aligned_realloc(ptr, newSize, alignment);
	}

	void SystemAllocator::Free(void* ptr)
	{
		_aligned_free(ptr);
	}
}
#endif
