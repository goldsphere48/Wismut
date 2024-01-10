#include "wipch.h"
#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Wi
{
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;

	void Logger::Initialize()
	{
		s_ClientLogger = spdlog::stdout_color_mt("Client");
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("Engine");
		s_EngineLogger->set_level(spdlog::level::trace);
		s_EngineLogger->set_pattern("%^[%T] %n: %v%$");
	}
}
