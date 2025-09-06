#pragma once

#include <map>
#include <stack>
#include <vector>

#include "Core/Memory/UntrackedAllocator.h"

namespace Wi
{
	template <typename Key, typename Value>
	using UntrackedMap = std::map<
		Key,
		Value,
		std::less<Key>,
		UntrackedAllocator<std::pair<const Key, Value>>
	>;

	template <typename T>
	using UntrackedVector = std::vector<T, UntrackedAllocator<T>>;

	template <typename T>
	using UntrackedStack = std::stack<T, std::deque<T, UntrackedAllocator<T>>>;
}
