#pragma once

#include <string_view>
#include <algorithm>
#include <array>

#include "Core/CoreTypes.h"

namespace Wi::Utils
{
	namespace Private
	{
		constexpr std::string_view ParseEnumName(std::string_view signature)
		{
#if defined(_MSC_VER)
			const char* startTag = "::";
			const char* endTag = ">";
#else
			const char* startTag = "= ";
			const char* endTag = "]";
#endif
			size_t start = signature.find_last_of(startTag) + 1;
			size_t end = signature.find_last_of(endTag);
			return signature.substr(start, end - start);
		}

		template<auto E>
		constexpr std::string_view GetEnumNameImpl()
		{
#if defined(_MSC_VER)
			return __FUNCSIG__;
#else
			return __PRETTY_FUNCTION__;
#endif
		}

		template<auto E>
		constexpr std::string_view GetEnumName()
		{
			constexpr std::string_view signature = GetEnumNameImpl<E>();
			return ParseEnumName(signature);
		}

		template<typename E, usize... Is>
		constexpr auto GenerateEnumNamesArray(std::integer_sequence<usize, Is...>)
		{
			return std::array<std::string_view, sizeof...(Is)> {
				GetEnumName<static_cast<E>(Is)>()...
			};
		}
	}


	template<typename E>
	inline const char* EnumToString(E value)
	{
		constexpr auto names = Private::GenerateEnumNamesArray<E>(
			std::make_index_sequence<static_cast<std::size_t>(E::Count)>{}
		);

		const auto index = static_cast<std::size_t>(value);
		if (index < names.size()) {
			std::string_view name = names[index];
			static thread_local char buffer[256];
			size_t len = std::min(name.length(), sizeof(buffer) - 1);
			std::copy_n(name.begin(), len, buffer);
			buffer[len] = '\0';
			return buffer;
		}
		return "Unknown";
	}
}
