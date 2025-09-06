#pragma once

#include "Core/Templates/Concepts.h"

namespace Wi
{
	struct Math
	{
		template<CIntegral T>
		static constexpr T Max(T a, T b)
		{
			return a >= b ? a : b;
		}

		template<CIntegral T>
		static constexpr T Min(T a, T b)
		{
			return a < b ? a: b;
		}

		template<CIntegral T>
		static constexpr bool IsPowOf2(T a)
		{
			return a != 0 && (a & (a - 1)) == 0;
		}
	};
}
