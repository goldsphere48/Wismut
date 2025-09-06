#include "Memory.h"

#include "DefaultMalloc.h"
#include "MemoryTracker.h"
#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"
#include "Core/Templates/AlignTemplates.h"
#include "Core/Math/Math.h"

namespace Wi
{
	static constexpr uint64 DEFAULT_ALIGNMENT = 16;

	void Memory::Initialize()
	{
		GetMalloc();
		MemoryTracker::Initialize();
	}

	void Memory::Shutdown()
	{
	}

	void* Memory::Allocate(uint64 size, uint64 alignment, const char* filename, int line)
	{
		alignment = Math::Max(DEFAULT_ALIGNMENT, alignment);

		if (!Math::IsPowOf2(alignment))
		{
			Log::Error("Alignment must be power of 2, actual alignment is {0}", alignment);
		}

		uint64 alignedSize = Align(size, alignment);

		void* ptr = GetMalloc().Allocate(alignedSize, alignment);

		MemoryTracker::TrackAllocation(ptr, alignedSize, filename, line);

		return ptr;
	}

	void Memory::Free(void* ptr)
	{
		MemoryTracker::TrackFree(ptr);
		GetMalloc().Free(ptr);
	}

	void* Memory::UntrackedSystemAlloc(uint64 size)
	{
		if (MemoryTracker::IsEnabled()) {
			int headerSize = sizeof(uint64);
			void* ptr = malloc(size + headerSize);
			*(static_cast<uint64*>(ptr)) = size;
			void* userPtr = static_cast<uint8*>(ptr) + headerSize;

			MemoryTracker::TrackUntrackedMemoryAllocation(size);

			return userPtr;
		}

		return malloc(size);
	}

	void Memory::UntrackedSystemFree(void* ptr)
	{
		if (MemoryTracker::IsEnabled())
		{
			int headerSize = sizeof(uint64);
			void* rawPtr = static_cast<uint8*>(ptr) - headerSize;
			uint64 size = *static_cast<uint64*>(rawPtr);

			MemoryTracker::TrackUntrackedMemoryFree(size);

			free(rawPtr);
			return;
		}

		free(ptr);
	}

	IMalloc& Memory::GetMalloc()
	{
		static DefaultMalloc malloc;
		return malloc;
	}

	void* _wi_memalloc(uint64 size, uint64 alignment, const char* filename, int line)
	{
		return Memory::Allocate(size, alignment, filename, line);
	}

	void _wi_memfree(void* ptr)
	{
		Memory::Free(ptr);
	}

}

void* operator new(size_t size)
{
	return Wi::Memory::Allocate(size);
}

void* operator new [](size_t size)
{
	return Wi::Memory::Allocate(size);
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
	try {
		return Wi::Memory::Allocate(size);
	}
	catch (...) {
		return nullptr;
	}
}

void* operator new [](std::size_t size, const std::nothrow_t&) noexcept
{
	try {
		return Wi::Memory::Allocate(size);
	}
	catch (...) {
		return nullptr;
	}
}

void* operator new(std::size_t size, std::align_val_t align)
{
	return Wi::Memory::Allocate(size, static_cast<uint64>(align));
}

void* operator new [](std::size_t size, std::align_val_t align)
{
	return Wi::Memory::Allocate(size, static_cast<uint64>(align));
}

void* operator new(std::size_t size, const char* filename, int line)
{
	return Wi::Memory::Allocate(size, 0, filename, line);
}

void* operator new [](std::size_t size, const char* filename, int line)
{
	return Wi::Memory::Allocate(size, 0, filename, line);
}

void operator delete(void* ptr) noexcept
{
	return Wi::Memory::Free(ptr);
}

void operator delete [](void* ptr) noexcept
{
	return Wi::Memory::Free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept
{
	return Wi::Memory::Free(ptr);
}

void operator delete [](void* ptr, const std::nothrow_t&) noexcept
{
	return Wi::Memory::Free(ptr);
}

void operator delete(void* ptr, std::align_val_t) noexcept
{
	return Wi::Memory::Free(ptr);
}

void operator delete [](void* ptr, std::align_val_t) noexcept
{
	return Wi::Memory::Free(ptr);
}

void operator delete(void* ptr, const char* filename, int line) noexcept
{
	return Wi::Memory::Free(ptr);
}

void operator delete [](void* ptr, const char* filename, int line) noexcept
{
	return Wi::Memory::Free(ptr);
}
