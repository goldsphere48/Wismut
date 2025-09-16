#pragma once

namespace Wi
{
	class DateTime;
	enum class LogLevel;
}

namespace Wi::Platform::Console
{
	void WriteLog(LogLevel level, const char* message);
}

namespace Wi::Platform::Time
{
	DateTime GetLocalTime();
}
