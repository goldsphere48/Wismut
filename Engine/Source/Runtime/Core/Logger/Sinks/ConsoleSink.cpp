#include "ConsoleSink.h"
#include "Platform/IPlatform.h"

namespace Wi
{
	void ConsoleSink::Log(LogLevel level, const char* message)
	{
		Platform::ConsoleWriteLog(level, message);
	}
}
