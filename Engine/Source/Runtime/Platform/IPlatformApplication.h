#pragma once

#include "Core/Memory/MemoryTypes.h"
#include "IPlatformWindow.h"

namespace Wi
{
	class IPlatformApplication
	{
	public:
		virtual ~IPlatformApplication() = default;
		virtual void Startup() = 0;
		virtual SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) = 0;
		virtual void PumpMessages() = 0;
		virtual void Shutdown() = 0;
	};
}
