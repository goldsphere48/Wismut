#ifdef WI_PLATFORM_ANDROID
#include "AndroidPlatform.h"
#include "Core/Time.h"

#include <android/log.h>
#include <ctime>
#include <sys/time.h>

#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"

namespace Wi::Platform::Console
{
	void WriteLog(LogLevel level, const char* message)
	{
		__android_log_write(ANDROID_LOG_INFO, "Wismut", message);
	}
}

namespace Wi::Platform::Time
{
	DateTime GetLocalTime()
	{
		timeval tv;
		gettimeofday(&tv, nullptr);

		tm local_tm;
		WI_ASSERT(localtime_r(&tv.tv_sec, &local_tm));

		uint64 milliseconds =
			local_tm.tm_hour * 3600000ULL +
			local_tm.tm_min * 60000ULL +
			local_tm.tm_sec * 1000ULL +
			tv.tv_usec / 1000;

		return TimeSpan::FromMilliseconds(milliseconds);
	}
}

namespace Wi
{
	void ErrorMessage(const char* str)
	{
		Platform::Console::WriteLog(LogLevel::Fatal, str);
	}
}

#endif
