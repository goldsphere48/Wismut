#ifdef WI_PLATFORM_LINUX
#include "LinuxConsole.h"

#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"

namespace Wi
{
	void LinuxConsole::ConsoleWriteLog(LogLevel level, const char* message)
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
#endif
