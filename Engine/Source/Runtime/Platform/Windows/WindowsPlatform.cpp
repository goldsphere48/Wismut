#ifdef WI_PLATFORM_WIN
#include "WindowsPlatform.h"

#include <Windows.h>
#include <cstring>

#include "Core/Assertion.h"
#include "Platform/IPlatform.h"
#include "WindowsApplication.h"
#include "Core/Logger/Logger.h"

enum class ConsoleForeground : WORD
{
	BLACK = 0,
	DARKBLUE	= FOREGROUND_BLUE,
	DARKGREEN	= FOREGROUND_GREEN,
	DARKCYAN	= FOREGROUND_GREEN | FOREGROUND_BLUE,
	DARKRED		= FOREGROUND_RED,
	DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
	DARKYELLOW  = FOREGROUND_RED | FOREGROUND_GREEN,
	DARKGRAY	= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	GRAY		= FOREGROUND_INTENSITY,
	BLUE		= FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	GREEN		= FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	CYAN		= FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	RED			= FOREGROUND_INTENSITY | FOREGROUND_RED,
	MAGENTA		= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	YELLOW		= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	WHITE		= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

namespace Wi::Platform
{
	UniquePtr<IPlatformApplication> CreateNativeApplication()
	{
		return CreateUniquePtr<WindowsApplication>();
	}

	void ConsoleWriteColoredLog(Wi::LogLevel level, const char* message)
	{
		ConsoleForeground color = ConsoleForeground::WHITE;
		switch (level)
		{
			case Wi::LogLevel::Verbose:
				color = ConsoleForeground::WHITE;
				break;
			case Wi::LogLevel::Info:
				color = ConsoleForeground::GREEN;
				break;
			case Wi::LogLevel::Debug:
				color = ConsoleForeground::CYAN;
				break;
			case Wi::LogLevel::Warning:
				color = ConsoleForeground::YELLOW;
				break;
			case Wi::LogLevel::Error:
				color = ConsoleForeground::RED;
				break;
			case Wi::LogLevel::Fatal:
				color = ConsoleForeground::DARKRED;
				break;
		}

		bool error = level == Wi::LogLevel::Error || level == Wi::LogLevel::Fatal;
		DWORD nStdHandle = error ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE;
		HANDLE handle = GetStdHandle(nStdHandle);
		CORE_CHECK(handle)
		CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
		GetConsoleScreenBufferInfo(handle, &consoleScreenBufferInfo);
		WORD attributesToRestore = consoleScreenBufferInfo.wAttributes;
		SetConsoleTextAttribute(handle, static_cast<WORD>(color));
		WriteConsoleA(handle, message, strlen(message), nullptr, nullptr);
		SetConsoleTextAttribute(handle, attributesToRestore);
	}

	DateTime GetLocalTime()
	{
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		u64 milliseconds =
			systemTime.wHour * 3600000ULL +
			systemTime.wMinute * 60000ULL +
			systemTime.wSecond * 1000ULL +
			systemTime.wMilliseconds;

		return DateTime::FromMilliseconds(milliseconds);
	}
}
#endif
