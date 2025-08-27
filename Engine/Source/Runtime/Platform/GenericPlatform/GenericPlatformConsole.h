#pragma once

namespace Wi
{
	enum class LogLevel;

	class GenericPlatformConsole
	{
	public:
		static void ConsoleWriteLog(LogLevel level, const char* message);
	};
}
