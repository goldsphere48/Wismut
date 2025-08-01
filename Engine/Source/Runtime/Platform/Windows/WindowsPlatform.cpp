#ifdef WI_PLATFORM_WIN
#include "WindowsPlatform.h"

#include <Windows.h>
#include <cstring>

#include "Core.h"
#include "Platform/IPlatform.h"
#include "WindowsApplication.h"

enum class ConsoleForeground : uint8
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

	void ConsoleWriteLog(LogLevel level, const char* message)
	{
		ConsoleForeground color = ConsoleForeground::WHITE;
		switch (level)
		{
			case LogLevel::Verbose:
			{
				color = ConsoleForeground::WHITE;
				break;
			}
			case LogLevel::Info:
			{
				color = ConsoleForeground::GREEN;
				break;
			}
			case LogLevel::Debug:
			{
				color = ConsoleForeground::CYAN;
				break;
			}
			case LogLevel::Warning:
			{
				color = ConsoleForeground::YELLOW;
				break;
			}
			case LogLevel::Error:
			{
				color = ConsoleForeground::RED;
				break;
			}
			case LogLevel::Fatal:
			{
				color = ConsoleForeground::DARKRED;
				break;
			}
		}

		bool error = level == LogLevel::Error || level == LogLevel::Fatal;
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

		uint64 milliseconds =
			systemTime.wHour * 3600000ULL +
			systemTime.wMinute * 60000ULL +
			systemTime.wSecond * 1000ULL +
			systemTime.wMilliseconds;

		return DateTime::FromMilliseconds(milliseconds);
	}

	void* Allocate(uint32 bufferSize)
	{
		HANDLE heap = GetProcessHeap();
		void* ptr = HeapAlloc(heap, HEAP_ZERO_MEMORY, bufferSize);
		CORE_CHECK(ptr)
		return ptr;
	}

	void Free(void* buffer)
	{
		HANDLE heap = GetProcessHeap();
		HeapFree(heap, 0, buffer);
	}
}
#endif
