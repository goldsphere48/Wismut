#pragma once

namespace Wi
{
	struct LogEntry;

	class ILoggerSink
	{
	public:
		virtual ~ILoggerSink() = default;
		virtual void Log(const LogEntry& logEntry) = 0;
	};
}
