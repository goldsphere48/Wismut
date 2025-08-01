#ifdef WI_PLATFORM_LINUX
#include "LinuxPlatform.h"

#include <sys/time.h>

#include "Core.h"
#include "Platform/IPlatform.h"
#include "LinuxApplication.h"

namespace Wi::Platform
{
	UniquePtr<IPlatformApplication> CreateNativeApplication()
	{
		return CreateUniquePtr<LinuxApplication>();
	}

	void ConsoleWriteLog(LogLevel level, const char* message)
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

		int errorCode = fprintf(stream, "%s%s\033[0m", colorCode, message);
		CORE_CHECK(!errorCode)

		errorCode = fflush(stream);
		CORE_CHECK(!errorCode)
	}

	DateTime GetLocalTime()
	{
		timeval tv;
		gettimeofday(&tv, nullptr);

		tm local_tm;
		CORE_CHECK(localtime_r(&tv.tv_sec, &local_tm));

		uint64 milliseconds =
			local_tm.tm_hour * 3600000ULL +
			local_tm.tm_min * 60000ULL +
			local_tm.tm_sec * 1000ULL +
			tv.tv_usec / 1000;

		return TimeSpan(milliseconds);
	}

	void* Allocate(uint32 bufferSize)
	{
		void* ptr = malloc(bufferSize);
		CORE_CHECK(ptr)
		return ptr;
	}

	void Free(void* buffer)
	{
		free(buffer);
	}
}
#endif
