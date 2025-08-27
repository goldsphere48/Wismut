#pragma once
#ifdef WI_PLATFORM_LINUX
#include "Platform/GenericPlatform/GenericPlatformConsole.h"

namespace Wi
{
	class LinuxConsole : public GenericPlatformConsole
	{
	public:
		static void ConsoleWriteLog(LogLevel level, const char* message);
	};

	using PlatformConsole = LinuxConsole;
}
#endif
