#pragma once
#include <format>
#include <vector>
#include <string>
#include <mutex>

#include "ILoggerSink.h"
#include "Core/Memory/Memory.h"

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
		Logger() = default;
		~Logger() = default;

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger& operator=(Logger&&) = delete;
		Logger(Logger&&) = delete;

		template<typename TSink, typename ...Args>
		void RegisterSink(Args&&... args)
		{
			std::scoped_lock lock(m_SinksMutex);
			m_Sinks.emplace_back(CreateUniquePtr<TSink>(std::forward<Args>(args)...));
		}

		static const Logger& GetEngineLogger();

		template<typename ...Args>
		void Log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) const
		{
			std::string message = std::format(fmt, std::forward<Args>(args)...);
			Log(level, message);
		}

		void Log(LogLevel level, std::string_view message) const;

	private:
		mutable std::mutex m_SinksMutex;
		std::vector<UniquePtr<ILoggerSink>> m_Sinks;
	};

	namespace Log
	{
		template<typename ...Args>
		inline void Verbose(std::format_string<Args...> fmt, Args&&... args)
		{
			Logger::GetEngineLogger().Log(LogLevel::Verbose, fmt, std::forward<Args>(args)...);
		}

		inline void Verbose(std::string_view message)
		{
			Logger::GetEngineLogger().Log(LogLevel::Verbose, message);
		}

		template<typename ...Args>
		inline void Info(std::format_string<Args...> fmt, Args&&... args)
		{
			Logger::GetEngineLogger().Log(LogLevel::Info, fmt, std::forward<Args>(args)...);
		}

		inline void Info(std::string_view message)
		{
			Logger::GetEngineLogger().Log(LogLevel::Info, message);
		}

		template<typename ...Args>
		inline void Debug(std::format_string<Args...> fmt, Args&&... args)
		{
			Logger::GetEngineLogger().Log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
		}

		inline void Debug(std::string_view message)
		{
			Logger::GetEngineLogger().Log(LogLevel::Debug, message);
		}

		template<typename ...Args>
		inline void Warn(std::format_string<Args...> fmt, Args&&... args)
		{
			Logger::GetEngineLogger().Log(LogLevel::Warning, fmt, std::forward<Args>(args)...);
		}

		inline void Warn(std::string_view message)
		{
			Logger::GetEngineLogger().Log(LogLevel::Warning, message);
		}

		template<typename ...Args>
		inline void Error(std::format_string<Args...> fmt, Args&&... args)
		{
			Logger::GetEngineLogger().Log(LogLevel::Error, fmt, std::forward<Args>(args)...);
		}

		inline void Error(std::string_view message)
		{
			Logger::GetEngineLogger().Log(LogLevel::Error, message);
		}

		template<typename ...Args>
		inline void Fatal(std::format_string<Args...> fmt, Args&&... args)
		{
			Logger::GetEngineLogger().Log(LogLevel::Fatal, fmt, std::forward<Args>(args)...);
		}

		inline void Fatal(std::string_view message)
		{
			Logger::GetEngineLogger().Log(LogLevel::Fatal, message);
		}
	}
}
