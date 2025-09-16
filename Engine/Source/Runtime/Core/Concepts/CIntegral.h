#pragma once

#include <type_traits>

namespace Wi
{
	template<typename T>
	concept CIntegral = std::is_integral_v<T>;
}
