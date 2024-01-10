#pragma once
#include <spdlog/spdlog.h>

namespace Wi
{
	class Logger
	{
	public:
		static void Initialize();

		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define WI_CORE_TRACE(message, ...) ::Wi::Logger::GetEngineLogger()->trace(message, __VA_ARGS__);
#define WI_CORE_INFO(message, ...) ::Wi::Logger::GetEngineLogger()->info(message, __VA_ARGS__);
#define WI_CORE_WARN(message, ...) ::Wi::Logger::GetEngineLogger()->warn(message, __VA_ARGS__);
#define WI_CORE_ERROR(message, ...) ::Wi::Logger::GetEngineLogger()->error(message, __VA_ARGS__);
#define WI_CORE_CRITICAL(message, ...) ::Wi::Logger::GetEngineLogger()->critical(message, __VA_ARGS__);

#define WI_TRACE(message, ...) ::Wi::Logger::GetClientLogger()->trace(message, __VA_ARGS__);
#define WI_INFO(message, ...) ::Wi::Logger::GetClientLogger()->info(message, __VA_ARGS__);
#define WI_WARN(message, ...) ::Wi::Logger::GetClientLogger()->warn(message, __VA_ARGS__);
#define WI_ERROR(message, ...) ::Wi::Logger::GetClientLogger()->error(message, __VA_ARGS__);
#define WI_CRITICAL(message, ...) ::Wi::Logger::GetClientLogger()->critical(message, __VA_ARGS__);