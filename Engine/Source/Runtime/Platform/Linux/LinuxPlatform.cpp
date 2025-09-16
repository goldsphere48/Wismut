#ifdef WI_PLATFORM_LINUX
#include "LinuxPlatform.h"

#include <ctime>
#include <sys/time.h>

#include "Core/Assertion.h"
#include "Core/Time.h"
#include "Core/Logger/Logger.h"

namespace Wi::Platform::Console
{
	void WriteLog(LogLevel level, const char* message)
	{
		const char* colorCode = "\033[0m";
		switch (level)
		{
			case LogLevel::Verbose:
			{
				colorCode = "\033[37m";
				break;
			}
			case LogLevel::Info:
			{
				colorCode = "\033[32m";
				break;
			}
			case LogLevel::Debug:
			{
				colorCode = "\033[36m";
				break;
			}
			case LogLevel::Warning:
			{
				colorCode = "\033[33m";
				break;
			}
			case LogLevel::Error:
			{
				colorCode = "\033[31m";
				break;
			}
			case LogLevel::Fatal:
			{
				colorCode = "\033[1;31m";
				break;
			}
		}

		bool error = (level == LogLevel::Error) || (level == LogLevel::Fatal);
		FILE* stream = error ? stderr : stdout;

		fprintf(stream, "%s%s\033[0m", colorCode, message);
		fflush(stream);
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
		Log::Error(str);
	}
}

#endif
