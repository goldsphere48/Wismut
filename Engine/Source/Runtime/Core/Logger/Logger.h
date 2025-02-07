#pragma once
#include <format>
#include <vector>
#include <string>

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

	struct ILoggerSink
	{
		virtual ~ILoggerSink() = default;
		virtual void Log(LogLevel level, const char* message) = 0;
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

	private:
		template<typename ...Args>
		void Log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) const
		{
			std::string message = std::format(fmt, std::forward<Args>(args)...);
			LogString(level, message.c_str());
		}

		void LogString(LogLevel level, const char* messageBody) const;

	private:
		std::vector<ILoggerSink*> m_Sinks;
	};
}


#include "Application/Application.h"
#define WI_CORE_VERBOSE(msg, ...)	::Wi::Application::InstancePtr()->GetCoreLoggerPtr()->Verbose(msg, ##__VA_ARGS__);
#define WI_CORE_INFO(msg, ...)		::Wi::Application::InstancePtr()->GetCoreLoggerPtr()->Info(msg, ##__VA_ARGS__);
#define WI_DEBUG_LOG(msg, ...)		::Wi::Application::InstancePtr()->GetCoreLoggerPtr()->Debug(msg, ##__VA_ARGS__);
#define WI_CORE_WARN(msg, ...)		::Wi::Application::InstancePtr()->GetCoreLoggerPtr()->Warn(msg, ##__VA_ARGS__);
#define WI_CORE_ERROR(msg, ...)		::Wi::Application::InstancePtr()->GetCoreLoggerPtr()->Error(msg, ##__VA_ARGS__);
#define WI_CORE_FATAL(msg, ...)		::Wi::Application::InstancePtr()->GetCoreLoggerPtr()->Fatal(msg, ##__VA_ARGS__);
