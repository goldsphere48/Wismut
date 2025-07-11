#include "Logger.h"

#include "Platform/IPlatform.h"
#include "Sinks/ConsoleSink.h"
#include "LogEntry.h"

namespace Wi
{
	const Logger& Logger::GetEngineLogger()
	{
		static Logger s_EngineLoggerInstance;
		static std::once_flag onceFlag;

		std::call_once(onceFlag, [&] {
			s_EngineLoggerInstance.RegisterSink<ConsoleSink>();
		});

		return s_EngineLoggerInstance;
	}

	void Logger::Log(LogLevel level, std::string_view messageBody) const
	{
		// TODO: Copy message body after make logger async
		LogEntry entry {
			.Level = level,
			.Message = messageBody,
			.Timestamp = Platform::GetLocalTime()
		};

		std::scoped_lock lock(m_SinksMutex);
		for (const auto& sink : m_Sinks)
		{
			sink->Log(entry);
		}
	}
}
