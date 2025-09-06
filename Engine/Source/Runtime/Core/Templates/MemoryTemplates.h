#pragma once

#include "Concepts.h"

namespace Wi
{
	template<CIntegral T>
	constexpr T KB(T kbs)
	{
		return 1024 * kbs;
	}

	template<CIntegral T>
	constexpr T MB(T mbs)
	{
		return 1024 * KB(mbs);
	}

	template<CIntegral T>
	constexpr T GB(T gbs)
	{
		return 1024 * MB(gbs);
	}
}
