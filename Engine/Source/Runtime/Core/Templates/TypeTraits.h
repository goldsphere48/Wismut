#pragma once

#include "Core/CoreTypes.h"

namespace Wi
{
	template<typename T, typename U>
	struct IsSame
	{
		static constexpr bool Value = false;
	};

	template<typename T>
	struct IsSame<T, T>
	{
		static constexpr bool Value = true;
	};

	template<typename T, typename U>
	inline constexpr bool IsSameValue = IsSame<T, U>::Value;

	template<typename T>
	struct RemoveCV
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<const T>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<volatile T>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveCV<const volatile T>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveVolatile
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveVolatile<volatile T>
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveVolatile<const volatile T>
	{
		using Type = const T;
	};

	template<typename T>
	using RemoveCVType = typename RemoveCV<T>::Type;

	template<typename T>
	struct RemoveReference
	{
		using Type = T;
	};

	template<typename T>
	struct RemoveReference<T&>
	{
		using Type = T;
	};
}
