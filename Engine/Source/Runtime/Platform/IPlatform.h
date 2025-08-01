#pragma once

/*------------------------------------------------------------------------------
	Platform common interface.
------------------------------------------------------------------------------*/

#include "Core/Memory/Memory.h"
#include "Core/CoreTypes.h"
#include "Core/Time.h"

namespace Wi
{
	enum class LogLevel;
	class IPlatformApplication;

	namespace Platform
	{
		UniquePtr<IPlatformApplication> CreateNativeApplication();

		void ConsoleWriteLog(LogLevel level, const char* message);

		DateTime GetLocalTime();

		void* Allocate(uint32 bufferSize);

		void Free(void* buffer);
	}
}
