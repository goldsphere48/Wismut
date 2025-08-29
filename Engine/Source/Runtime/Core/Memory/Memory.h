#pragma once
#include <new>
#include <utility>

#include "Core/CoreTypes.h"
#include "MemoryBase.h"

namespace Wi
{
	class Memory
	{
	public:
		static void Initialize();
		static void Shutdown();
		static void* Allocate(usize size, usize alignment = 0);
		static void Free(void* ptr);

	private:
		static IMalloc& GetMalloc();
	};
}

void* operator new  (std::size_t size);
void* operator new[](std::size_t size);
void* operator new  (std::size_t size, const std::nothrow_t&) noexcept;
void* operator new[](std::size_t size, const std::nothrow_t&) noexcept;
void* operator new  (std::size_t size, std::align_val_t al);
void* operator new[](std::size_t size, std::align_val_t al);
void* operator new  (std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept;
void* operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept;

void operator delete  (void* p) noexcept;
void operator delete[](void* p) noexcept;
void operator delete  (void* p, std::size_t, std::align_val_t) noexcept;
void operator delete[](void* p, std::size_t, std::align_val_t) noexcept;
void operator delete  (void* p, std::size_t) noexcept;
void operator delete[](void* p, std::size_t) noexcept;
void operator delete  (void* p, std::align_val_t) noexcept;
void operator delete[](void* p, std::align_val_t) noexcept;
