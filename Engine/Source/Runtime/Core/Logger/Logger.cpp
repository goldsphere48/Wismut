#include "Logger.h"

#include "Sinks/ConsoleSink.h"
#include "Platform/IPlatform.h"

namespace Wi
{
	const char* LevelToString(LogLevel level)
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
