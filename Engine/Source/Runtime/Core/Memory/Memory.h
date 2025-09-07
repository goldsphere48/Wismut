#pragma once

#include <new>

#include "Core/CoreTypes.h"

void* operator new  (std::size_t size, const char* filename, int line);
void* operator new[](std::size_t size, const char* filename, int line);

void operator delete  (void* ptr, const char* filename, int line) noexcept;
void operator delete[](void* ptr, const char* filename, int line) noexcept;

namespace Wi
{
	#define WI_NEW new(__FILE__, __LINE__)
	#define WI_ALLOC(size) _wi_memalloc(size, 0, __FILE__, __LINE__)
	#define WI_ALLOC_ALIGNED(size, alignment) _wi_memalloc(size, alignment, __FILE__, __LINE__)
	#define WI_FREE(ptr) _wi_memfree(ptr)

	namespace Private
	{
		void* WMalloc(uint64 size, uint64 alignment = 0, const char* filename = nullptr, int line = 0);
		void WFree(void* ptr);
	}

	struct IMalloc;

	class Memory
	{
	public:
		static void Initialize();
		static void Shutdown();

	private:
		static void* UntrackedSystemAlloc(uint64 size);
		static void UntrackedSystemFree(void* ptr);

		static void* Allocate(uint64 size, uint64 alignment = 0, const char* filename = nullptr, int line = 0);
		static void Free(void* ptr);

		static IMalloc& GetMalloc();

		friend void* ::operator new  (std::size_t size);
		friend void* ::operator new[](std::size_t size);
		friend void* ::operator new  (std::size_t size, const std::nothrow_t&) noexcept;
		friend void* ::operator new[](std::size_t size, const std::nothrow_t&) noexcept;
		friend void* ::operator new  (std::size_t size, std::align_val_t);
		friend void* ::operator new[](std::size_t size, std::align_val_t);
		friend void* ::operator new  (std::size_t size, const char* filename, int line);
		friend void* ::operator new[](std::size_t size, const char* filename, int line);

		friend void ::operator delete  (void* ptr) noexcept;
		friend void ::operator delete[](void* ptr) noexcept;
		friend void ::operator delete  (void* ptr, const std::nothrow_t&) noexcept;
		friend void ::operator delete[](void* ptr, const std::nothrow_t&) noexcept;
		friend void ::operator delete  (void* ptr, std::align_val_t) noexcept;
		friend void ::operator delete[](void* ptr, std::align_val_t) noexcept;
		friend void ::operator delete  (void* ptr, const char* filename, int line) noexcept;
		friend void ::operator delete[](void* ptr, const char* filename, int line) noexcept;

		friend void* Private::WMalloc(uint64 size, uint64 alignment, const char* filename, int line);
		friend void Private::WFree(void* ptr);

		template<typename T>
		friend class UntrackedAllocator;
	};
}
