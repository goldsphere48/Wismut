#pragma once
#include <format>
#include <vector>
#include <string>

#include "ILoggerSink.h"

namespace Wi
{
	enum class LogLevel
	{
		Verbose,
		Info,
		Debug,
		Warning,
		Error,
		Fatal,
	};

	class Logger
	{
	public:
		~Logger();

		template<typename TSink, typename ...Args>
		void RegisterSink(Args&&... args)
		{
			TSink* sink = new TSink(std::forward<Args>(args)...);
			m_Sinks.emplace_back(sink);
		}

		template<typename ...Args>
		void Verbose(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LogLevel::Verbose, fmt, std::forward<Args>(args)...);
		}

		void Verbose(const char* message) const
		{
			LogString(LogLevel::Verbose, message);
		}

		template<typename ...Args>
		void Info(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LogLevel::Info, fmt, std::forward<Args>(args)...);
		}

		void Info(const char* message) const
		{
			LogString(LogLevel::Info, message);
		}

		template<typename ...Args>
		void Debug(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
		}

		void Debug(const char* message) const
		{
			LogString(LogLevel::Debug, message);
		}

		template<typename ...Args>
		void Warn(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LogLevel::Warning, fmt, std::forward<Args>(args)...);
		}

		void Warn(const char* message) const
		{
			LogString(LogLevel::Warning, message);
		}

		template<typename ...Args>
		void Error(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LogLevel::Error, fmt, std::forward<Args>(args)...);
		}

		void Error(const char* message) const
		{
			LogString(LogLevel::Error, message);
		}

		template<typename ...Args>
		void Fatal(std::format_string<Args...> fmt, Args&&... args) const
		{
			Log(LogLevel::Fatal, fmt, std::forward<Args>(args)...);
		}

		void Fatal(const char* message) const
		{
			LogString(LogLevel::Fatal, message);
		}

		static void Initialize();
		static void Shutdown();
		static const Logger* GetEngineLoggerPtr();

	private:
		template<typename ...Args>
		void Log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) const
		{
			std::string message = std::format(fmt, std::forward<Args>(args)...);
			LogString(level, message.c_str());
		}

		void LogString(LogLevel level, const char* messageBody) const;

	private:
		static Logger* s_EngineLogger;
		std::vector<ILoggerSink*> m_Sinks;
	};
}

#define WI_CORE_VERBOSE(msg, ...)	::Wi::Logger::GetEngineLoggerPtr()->Verbose(msg, ##__VA_ARGS__);
#define WI_CORE_INFO(msg, ...)		::Wi::Logger::GetEngineLoggerPtr()->Info(msg, ##__VA_ARGS__);
#define WI_DEBUG_LOG(msg, ...)		::Wi::Logger::GetEngineLoggerPtr()->Debug(msg, ##__VA_ARGS__);
#define WI_CORE_WARN(msg, ...)		::Wi::Logger::GetEngineLoggerPtr()->Warn(msg, ##__VA_ARGS__);
#define WI_CORE_ERROR(msg, ...)		::Wi::Logger::GetEngineLoggerPtr()->Error(msg, ##__VA_ARGS__);
#define WI_CORE_FATAL(msg, ...)		::Wi::Logger::GetEngineLoggerPtr()->Fatal(msg, ##__VA_ARGS__);
