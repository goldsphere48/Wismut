#pragma once

namespace Wi::Math
{
	template<typename T>
	static constexpr T Max(T a, T b) noexcept
	{
		return a >= b ? a : b;
	}
}
