#pragma once
#include "IAllocator.h"

namespace Wi
{
	class SystemAllocator final : public IAllocator
	{
	public:
		void* Allocate(usize size, usize alignment) override;
		void* Reallocate(void* ptr, usize oldSize, usize newSize, usize alignment) override;
		void  Free(void* ptr) override;
	};
}
