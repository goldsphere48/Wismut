#include "GenericPlatformTime.h"

#include <chrono>
#include "Core/Assertion.h"

namespace Wi
{
	DateTime GenericPlatformTime::GetLocalTime()
	{
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);

		std::tm local_tm = *std::localtime(&now_c);

		uint64_t milliseconds =
			static_cast<uint64_t>(local_tm.tm_hour) * 3600000ULL +
			static_cast<uint64_t>(local_tm.tm_min) * 60000ULL +
			static_cast<uint64_t>(local_tm.tm_sec) * 1000ULL;

		auto since_midnight = now - std::chrono::system_clock::from_time_t(std::mktime(&local_tm));

		milliseconds += std::chrono::duration_cast<std::chrono::milliseconds>(since_midnight).count() % 1000;

		return TimeSpan::FromMilliseconds(milliseconds);
	}
}
