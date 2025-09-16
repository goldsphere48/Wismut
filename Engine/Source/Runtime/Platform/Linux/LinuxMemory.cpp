#ifdef WI_PLATFORM_LINUX
#include "Core/Memory/SystemAllocator.h"

#include <cstdlib>
#include <memory.h>
#include <malloc.h>

#include "Core/Assertion.h"
#include "Core/Math/Math.h"

namespace Wi
{
	void* SystemAllocator::Allocate(usize size, usize alignment)
	{
		WI_ASSERT(Math::IsPowOf2(alignment))

		usize alignedSize = Math::AlignUp(size, alignment);

		return std::aligned_alloc(alignment, alignedSize);
	}

	void* SystemAllocator::Reallocate(void* ptr, usize oldSize, usize newSize, usize alignment)
	{
		WI_ASSERT(Math::IsPowOf2(alignment))
		WI_ASSERT(alignment >= sizeof(void*))

		if (newSize == 0) {
			if (ptr)
				free(ptr);

			return nullptr;
		}

		void* result = nullptr;

		if (posix_memalign(&result, alignment, newSize) != 0)
			return nullptr;

		if (ptr)
		{
			if (const usize toCopy = (oldSize < newSize) ? oldSize : newSize)
				memcpy(result, ptr, toCopy);

			free(ptr);
		}

		return result;
	}

	void SystemAllocator::Free(void* ptr)
	{
		std::free(ptr);
	}
}
#endif
