#include "Logger.h"

#include "Platform/IPlatform.h"
#include "Sinks/ConsoleSink.h"

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

	Logger* Logger::s_EngineLogger = nullptr;

	void Logger::Initialize()
	{
		s_EngineLogger = new Logger;
		s_EngineLogger->RegisterSink<ConsoleSink>();
	}

	void Logger::Shutdown()
	{
		delete s_EngineLogger;
	}

	const Logger* Logger::GetEngineLoggerPtr()
	{
		return s_EngineLogger;
	}

	Logger::~Logger()
	{
		for (const auto& sink : m_Sinks)
		{
			delete sink;
		}
	}

	void Logger::LogString(LogLevel level, const char* messageBody) const
	{
		const char* levelName = LevelToString(level);
		DateTime time = Platform::GetLocalTime();
		std::string message = std::format("[{0}:{1}:{2}][{3}] {4}\n", time.GetHours(), time.GetMinutes(), time.GetSeconds(), levelName, messageBody);
		for (const auto& sink : m_Sinks)
		{
			sink->Log(level, message.c_str());
		}
	}
}
