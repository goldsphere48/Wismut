#ifdef WI_PLATFORM_ANDROID
#include "Core/Memory/SystemAllocator.h"

#include <cstdlib>   // malloc, free, realloc, posix_memalign
#include <memory.h>  // memcpy
#include <malloc.h>

#include "Core/Assertion.h"
#include "Core/Math/Math.h"

namespace Wi
{
	void* SystemAllocator::Allocate(usize size, usize alignment)
	{

		WI_ASSERT(Math::IsPowOf2(alignment))
		WI_ASSERT(alignment >= sizeof(void*))

		if (size == 0)
			return nullptr;

		void* ptr = nullptr;
		const int rc = ::posix_memalign(&ptr, alignment, size);
		WI_ASSERT(rc == 0, "posix_memalign failed (rc=%d, align=%zu, size=%zu)", rc, (size_t)alignment, (size_t)size);
		return (rc == 0) ? ptr : nullptr;
	}

	void* SystemAllocator::Reallocate(void* ptr, usize oldSize, usize newSize, usize alignment)
	{
		if (ptr == nullptr)
			return Allocate(newSize, alignment);

		if (newSize == 0) {
			Free(ptr);
			return nullptr;
		}

		const usize kDefaultAlign = alignof(std::max_align_t);
		if (alignment == 0 || alignment <= kDefaultAlign) {
			void* newPtr = std::realloc(ptr, newSize);
			WI_ASSERT(newPtr != nullptr, "realloc failed (size=%zu)", (size_t)newSize);
			return newPtr;
		}

		void* newPtr = Allocate(newSize, alignment);
		WI_ASSERT(newPtr != nullptr, "Allocate failed in Reallocate (size=%zu, align=%zu)", (size_t)newSize, (size_t)alignment);

		const usize copySize = Math::Min(oldSize, newSize);
		if (copySize)
			std::memcpy(newPtr, ptr, (size_t)copySize);

		Free(ptr);
		return newPtr;
	}

	void SystemAllocator::Free(void* ptr)
	{
		std::free(ptr);
	}
}
#endif
