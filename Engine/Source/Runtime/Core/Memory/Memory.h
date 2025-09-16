#pragma once
#include "IAllocator.h"
#include "MemoryTypes.h"
#include "Core/CoreTypes.h"

#define WI_ALLOC(size)											::Wi::Private::Allocate(size, 0, __FILE__, __LINE__)
#define WI_ALLOC_ALIGNED(size, alignment)						::Wi::Private::Allocate(size, alignment, __FILE__, __LINE__)
#define WI_REALLOC(oldPtr, oldSize, newSize)					::Wi::Private::Reallocate(oldPtr, oldSize, newSize, 0, __FILE__, __LINE__)
#define WI_REALLOC_ALIGNED(oldPtr, oldSize, newSize, alignment)	::Wi::Private::Reallocate(oldPtr, oldSize, newSize, alignment, __FILE__, __LINE__)
#define WI_FREE(ptr)											::Wi::Private::Free(ptr)
#define WI_NEW													new(__FILE__, __LINE__)
#define WI_DELETE(ptr)											operator delete(ptr, __FILE__, __LINE__)
#define WI_DELETE_ARR(ptr)										operator delete[](ptr, __FILE__, __LINE__)

void* operator new  (std::size_t size, const char* filename, int line);
void* operator new[](std::size_t size, const char* filename, int line);

void operator delete  (void* ptr, const char* filename, int line) noexcept;
void operator delete[](void* ptr, const char* filename, int line) noexcept;

namespace Wi
{
	namespace Private
	{
		void* Allocate(usize size, usize alignment, const char* filename, int line);
		void* Reallocate(void* oldPtr, usize oldSize, usize newSize, usize alignment, const char* filename, int line);
		void  Free(void* ptr);
	}

	class Memory
	{
	public:
		void Initialize();
		void Shutdown();

		static Memory* GetInstance()
		{
			static Memory instance = Memory();
			return &instance;
		}

	private:
		Memory() = default;

		void* Allocate(usize size, usize alignment, const char* filename, int line) const;
		void* Reallocate(void* oldPtr, usize oldSize, usize newSize, usize alignment, const char* filename, int line) const;
		void  Free(void* ptr) const;
		
		friend void* Private::Allocate(usize size, usize alignment, const char* filename, int line);
		friend void* Private::Reallocate(void* oldPtr, usize oldSize, usize newSize, usize alignment, const char* filename, int line);
		friend void  Private::Free(void* ptr);

	private:
		UniquePtr<IAllocator> m_MainAllocator;
		static bool s_Initialized;
	};
}
