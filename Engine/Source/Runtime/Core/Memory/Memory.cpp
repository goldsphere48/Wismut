// TODO: Сделать все потокобезопасным
#include "Memory.h"

#include "MemoryTracker.h"
#include "SystemAllocator.h"
#include "Core/Assertion.h"
#include "Core/Math/Math.h"

namespace Wi
{
	constexpr usize MinAlignment = alignof(std::max_align_t);
	bool Memory::s_Initialized = false;

	void Memory::Initialize()
	{
		WI_ASSERT(!s_Initialized)

		s_Initialized = true;

		m_MainAllocator = CreateUniquePtr<SystemAllocator>();
	}

	void Memory::Shutdown()
	{
		WI_ASSERT(s_Initialized)

		s_Initialized = false;
	}

	void* Memory::Allocate(usize size, usize alignment, const char* filename, int line) const
	{
		WI_ASSERT(s_Initialized)
		WI_ASSERT(alignment == 0 || Math::IsPowOf2(alignment))

		if (alignment <= 0)
			alignment = MinAlignment;
		
		void* ptr = m_MainAllocator->Allocate(size, alignment);

		MemoryTracker::GetInstance()->TrackAllocation(ptr, size, nullptr, 0);

		return ptr;
	}

	void* Memory::Reallocate(void* oldPtr, usize oldSize, usize newSize, usize alignment, const char* filename, int line) const
	{
		WI_ASSERT(s_Initialized)
		WI_ASSERT(alignment == 0 || Math::IsPowOf2(alignment))

		if (alignment <= 0)
			alignment = MinAlignment;

		void* ptr = m_MainAllocator->Reallocate(oldPtr, oldSize, newSize, alignment);

		MemoryTracker::GetInstance()->TrackReallocation(oldPtr, ptr, newSize, nullptr, 0);

		return ptr;
	}

	void Memory::Free(void* ptr) const
	{
		WI_ASSERT(s_Initialized)

		m_MainAllocator->Free(ptr);

		MemoryTracker::GetInstance()->TrackFree(ptr);
	}

	namespace Private
	{
		void* Allocate(usize size, usize alignment, const char* filename, int line)
		{
			return Memory::GetInstance()->Allocate(size, alignment, filename, line);
		}

		void* Reallocate(void* oldPtr, usize oldSize, usize newSize, usize alignment, const char* filename, int line)
		{
			return Memory::GetInstance()->Reallocate(oldPtr, oldSize, newSize, alignment, filename, line);
		}

		void  Free(void* ptr)
		{
			return Memory::GetInstance()->Free(ptr);
		}
	}
}

void* operator new(std::size_t size, const char* filename, int line)
{
	return Wi::Private::Allocate(size, 0, filename, line);
}

void* operator new[](std::size_t size, const char* filename, int line)
{
	return Wi::Private::Allocate(size, 0, filename, line);
}

void operator delete(void* ptr, const char* filename, int line) noexcept
{
	Wi::Private::Free(ptr);
}

void operator delete[](void* ptr, const char* filename, int line) noexcept
{
	Wi::Private::Free(ptr);
}
