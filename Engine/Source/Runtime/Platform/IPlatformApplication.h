#pragma once
#include "Core.h"
#include "IPlatformWindow.h"

namespace Wi
{
	class IPlatformApplication
	{
	public:
		RULE_OF_FIVE(IPlatformApplication)

		virtual ~IPlatformApplication() = default;
		virtual bool Startup() = 0;
		virtual SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) = 0;
		virtual void PumpMessages() = 0;
		virtual void Shutdown() = 0;
	};
}
