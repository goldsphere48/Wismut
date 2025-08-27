#include "ConsoleSink.h"

#include <format>

#include "Platform/Platform.h"
#include "Core/Logger/LogEntry.h"
#include "Core/Logger/Logger.h"

namespace Wi
{
	static const char* LevelToString(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Verbose:
			return "Verbose";
		case LogLevel::Info:
			return "Info";
		case LogLevel::Debug:
			return "Debug";
		case LogLevel::Warning:
			return "Warning";
		case LogLevel::Error:
			return "Error";
		case LogLevel::Fatal:
			return "Fatal";
		}
		return "";
	}

	void ConsoleSink::Log(const LogEntry& entry)
	{
		std::string formattedMessage = std::format(
			"[{0}:{1}:{2}][{3}] {4}\n",
			entry.Timestamp.GetHours(),
			entry.Timestamp.GetMinutes(),
			entry.Timestamp.GetSeconds(),
			LevelToString(entry.Level),
			entry.Message
		);

		PlatformConsole::ConsoleWriteLog(entry.Level, formattedMessage.c_str());
	}
}
