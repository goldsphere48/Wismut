#pragma once
#include <string_view>

#include "Core/Time.h"

namespace Wi
{
	enum class LogLevel;

	struct LogEntry
	{
		LogLevel Level;
		std::string_view Message;
		DateTime Timestamp;
	};
}
