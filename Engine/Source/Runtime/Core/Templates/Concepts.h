#pragma once
#include "Core/Templates/TypeTraits.h"

namespace Wi
{
	template<typename T>
	concept CIntegral =
		IsSameValue<RemoveCVType<T>, bool> ||
		IsSameValue<RemoveCVType<T>, char> ||
		IsSameValue<RemoveCVType<T>, signed char> ||
		IsSameValue<RemoveCVType<T>, unsigned char> ||
		IsSameValue<RemoveCVType<T>, wchar_t> ||
		IsSameValue<RemoveCVType<T>, char16_t> ||
		IsSameValue<RemoveCVType<T>, char32_t> ||
		IsSameValue<RemoveCVType<T>, short> ||
		IsSameValue<RemoveCVType<T>, unsigned short> ||
		IsSameValue<RemoveCVType<T>, int> ||
		IsSameValue<RemoveCVType<T>, unsigned int> ||
		IsSameValue<RemoveCVType<T>, long> ||
		IsSameValue<RemoveCVType<T>, unsigned long> ||
		IsSameValue<RemoveCVType<T>, long long> ||
		IsSameValue<RemoveCVType<T>, unsigned long long> ||
		IsSameValue<RemoveCVType<T>, uint8> ||
		IsSameValue<RemoveCVType<T>, uint16> ||
		IsSameValue<RemoveCVType<T>, uint32> ||
		IsSameValue<RemoveCVType<T>, uint64>;
}
