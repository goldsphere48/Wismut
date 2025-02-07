#pragma once

/*------------------------------------------------------------------------------
	Platform common interface.
------------------------------------------------------------------------------*/

#include "Core.h"
#include "Core/Time.h"
#include "Platform/IPlatformApplication.h"


namespace Wi
{
	enum class LogLevel;

	namespace Platform
	{
		UniquePtr<IPlatformApplication> CreateNativeApplication();
		void ConsoleWriteColoredLog(LogLevel level, const char* message);
		DateTime GetLocalTime();
	}
}
