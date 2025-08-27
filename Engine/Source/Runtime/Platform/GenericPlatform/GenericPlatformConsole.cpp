#include "GenericPlatformConsole.h"

#include <iostream>

#include "Core/Logger/Logger.h"

namespace Wi
{
	void GenericPlatformConsole::ConsoleWriteLog(LogLevel level, const char* message)
	{
		const char* color_code = "\033[0m";
		const char* reset_code = "\033[0m";

		switch (level)
		{
		case LogLevel::Verbose:
			color_code = "\033[37m";
			break;
		case LogLevel::Info:
			color_code = "\033[32m";
			break;
		case LogLevel::Debug:
			color_code = "\033[36m";
			break;
		case LogLevel::Warning:
			color_code = "\033[33m";
			break;
		case LogLevel::Error:
			color_code = "\033[31m";
			break;
		case LogLevel::Fatal:
			color_code = "\033[31;1m";
			break;
		}

		std::ostream& output = (level == LogLevel::Error || level == LogLevel::Fatal) ? std::cerr : std::cout;
		output << color_code << message << reset_code << '\n';
	}
}
