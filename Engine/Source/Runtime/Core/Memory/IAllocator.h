#pragma once

#include "Core/CoreTypes.h"

namespace Wi
{
	class IAllocator
	{
	public:
		virtual ~IAllocator() = default;

		virtual void* Allocate(usize size, usize alignment) = 0;
		virtual void* Reallocate(void* ptr, usize oldSize, usize newSize, usize alignment) = 0;
		virtual void  Free(void* ptr) = 0;
	};
}
