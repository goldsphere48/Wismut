#pragma once

#include "Core/Memory/Memory.h"
#include "IPlatformWindow.h"

namespace Wi
{
	class IPlatformApplication
	{
	public:
		DELETE_COPYING(IPlatformApplication)

		virtual ~IPlatformApplication() = default;
		virtual bool Startup() = 0;
		virtual SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) = 0;
		virtual void PumpMessages() = 0;
		virtual void Shutdown() = 0;
	};
}
