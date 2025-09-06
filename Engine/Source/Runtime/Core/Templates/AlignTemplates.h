#pragma once

#include "Concepts.h"

namespace Wi
{
	template<CIntegral T>
	constexpr T Align(T size, T alignment)
	{
		return (size + alignment - 1) & ~(alignment - 1);
	}
}
