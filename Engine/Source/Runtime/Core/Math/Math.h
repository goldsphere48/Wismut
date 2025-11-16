#pragma once

#include "Core/Concepts/CIntegral.h"
#include "GLMWrapper.h"

namespace Wi::Math
{
	template<CIntegral T>
	constexpr T Max(T a, T b)
	{
		return a >= b ? a : b;
	}

	template<CIntegral T>
	constexpr T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	template<CIntegral T>
	constexpr bool IsPowOf2(T a)
	{
		return a != 0 && (a & (a - 1)) == 0;
	}

	template<CIntegral T>
	constexpr T AlignUp(T size, T alignment)
	{
		return (size + alignment - 1) & ~(alignment - 1);
	}
}
