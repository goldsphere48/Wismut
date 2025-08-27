#pragma once
#if WI_PLATFORM_WIN

#include "Platform/GenericPlatform/GenericPlatformConsole.h"

namespace Wi
{
	class WindowsConsole : public GenericPlatformConsole
	{
	public:
		static void ConsoleWriteLog(LogLevel level, const char* message);
	};

	using PlatformConsole = WindowsConsole;
}
#endif
