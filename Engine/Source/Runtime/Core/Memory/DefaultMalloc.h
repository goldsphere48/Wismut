#pragma once

#include "IMalloc.h"

namespace Wi
{
	class DefaultMalloc final : public IMalloc
	{
	public:
		void* Allocate(uint64 size, uint64 alignment) override;
		void Free(void* ptr) override;
	};
}
