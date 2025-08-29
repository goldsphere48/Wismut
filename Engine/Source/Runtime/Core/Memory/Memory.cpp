#include "Memory.h"

#include "MemoryBase.h"
#include "SystemMalloc.h"
#include "Core/Assertion.h"

namespace Wi
{
	static void* SystemAllocate(usize size, usize alignment = 0)
	{
		return PlatformMemory::AlignedAlloc(size, alignment);
	}

	static void SystemFree(void* ptr)
	{
		PlatformMemory::AlignedFree(ptr);
	}

	void* UseSystemMalloc::operator new(usize size)
	{
		return SystemAllocate(size);
	}

	void* UseSystemMalloc::operator new[](usize size)
	{
		return SystemAllocate(size);
	}

	void UseSystemMalloc::operator delete(void* p) noexcept
	{
		SystemFree(p);
	}

	void UseSystemMalloc::operator delete[](void* p) noexcept
	{
		SystemFree(p);
	}

	void Memory::Initialize()
	{
		GetMalloc();
	}

	void Memory::Shutdown()
	{

	}

	void* Memory::Allocate(usize size, usize alignment)
	{
		return GetMalloc().Allocate(size, alignment);
	}

	void Memory::Free(void* ptr)
	{
		GetMalloc().Free(ptr);
	}

	IMalloc& Memory::GetMalloc()
	{
		static SystemMalloc malloc;
		return malloc;
	}
}

void* operator new(usize size)
{
	return  Wi::Memory::Allocate(size);
}

void* operator new[](usize size)
{
	void* ptr = Wi::Memory::Allocate(size);
	if (ptr == nullptr && size != 0)
	{
		throw std::bad_alloc();
	}
	return ptr;
}

void* operator new(usize size, const std::nothrow_t&) noexcept
{
	return Wi::Memory::Allocate(size);
}

void* operator new[](usize size, const std::nothrow_t&) noexcept
{
	return Wi::Memory::Allocate(size);
}

void* operator new(usize size, std::align_val_t al)
{
	void* ptr = Wi::Memory::Allocate(size, static_cast<usize>(al));
	if (ptr == nullptr && size != 0)
	{
		throw std::bad_alloc();
	}
	return ptr;
}

void* operator new[](usize size, std::align_val_t al)
{
	void* ptr = Wi::Memory::Allocate(size, static_cast<usize>(al));
	if (ptr == nullptr && size != 0)
	{
		throw std::bad_alloc();
	}
	return ptr;
}

void* operator new(usize size, std::align_val_t al, const std::nothrow_t&) noexcept
{
	return Wi::Memory::Allocate(size, static_cast<usize>(al));
}

void* operator new[](usize size, std::align_val_t al, const std::nothrow_t&) noexcept
{
	return Wi::Memory::Allocate(size, static_cast<usize>(al));
}

void operator delete(void* p) noexcept
{
	Wi::Memory::Free(p);
}

void operator delete[](void* p) noexcept
{
	Wi::Memory::Free(p);
}

void operator delete(void* p, usize) noexcept
{
	Wi::Memory::Free(p);
}

void operator delete[](void* p, usize) noexcept
{
	Wi::Memory::Free(p);
}

void operator delete(void* p, std::align_val_t) noexcept
{
	Wi::Memory::Free(p);
}

void operator delete[](void* p, std::align_val_t) noexcept
{
	Wi::Memory::Free(p);
}

void operator delete(void* p, usize, std::align_val_t) noexcept
{
	Wi::Memory::Free(p);
}

void operator delete[](void* p, usize, std::align_val_t) noexcept
{
	Wi::Memory::Free(p);
}
