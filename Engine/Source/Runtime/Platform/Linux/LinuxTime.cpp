#include "LinuxTime.h"

#include <sys/time.h>

#include "Core/Assertion.h"

namespace Wi
{
	DateTime LinuxTime::GetLocalTime()
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

		return TimeSpan::FromMilliseconds(milliseconds);
	}
}
