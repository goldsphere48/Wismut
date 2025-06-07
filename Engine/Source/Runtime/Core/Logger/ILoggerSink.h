#pragma once

namespace Wi
{
	enum class LogLevel;

	struct ILoggerSink
	{
		virtual ~ILoggerSink() = default;
		virtual void Log(LogLevel level, const char* message) = 0;
	};
}