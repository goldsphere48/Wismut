#pragma once

#include "Core/Memory/Memory.h"

namespace Wi
{
	template <typename T>
	class UntrackedAllocator {
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		UntrackedAllocator() noexcept = default;
		template <class U>
		UntrackedAllocator(const UntrackedAllocator<U>&) noexcept {}

		
		T* allocate(std::size_t n) {
			const size_t bytes_to_alloc = n * sizeof(T);
			return static_cast<T*>(Memory::UntrackedSystemAlloc(bytes_to_alloc));
		}

		void deallocate(T* p, std::size_t n) noexcept {
			Memory::UntrackedSystemFree(p);
		}
	};

	template <class T, class U>
	bool operator==(const UntrackedAllocator<T>&, const UntrackedAllocator<U>&) {
		return true;
	}

	template <class T, class U>
	bool operator!=(const UntrackedAllocator<T>&, const UntrackedAllocator<U>&) {
		return false;
	}
}
