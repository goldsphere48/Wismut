#pragma once

#include "Core/CoreTypes.h"

namespace Wi
{
	struct IMalloc
	{
		virtual ~IMalloc() = default;
		virtual void* Allocate(uint64 size, uint64 alignment) = 0;
		virtual void Free(void* ptr) = 0;
	};
}
