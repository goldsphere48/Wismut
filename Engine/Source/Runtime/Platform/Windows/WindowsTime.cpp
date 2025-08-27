#if WI_PLATFORM_WIN
#include "WindowsTime.h"

#include <Windows.h>

#include "Core/Assertion.h"

namespace Wi
{
	DateTime WindowsTime::GetLocalTime()
	{
		SYSTEMTIME systemTime;
		::GetLocalTime(&systemTime);

		uint64 milliseconds =
			systemTime.wHour * 3600000ULL +
			systemTime.wMinute * 60000ULL +
			systemTime.wSecond * 1000ULL +
			systemTime.wMilliseconds;

		return DateTime::FromMilliseconds(milliseconds);
	}
}
#endif
