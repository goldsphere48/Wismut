#pragma once
#include "IPlatformWindow.hpp"
#include "Core/Defines.hpp"

namespace Wi
{
	class IPlatformApplication
	{
	public:
		virtual ~IPlatformApplication() = default;
		virtual bool Startup() = 0;
		virtual SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) = 0;
		virtual void PumpMessages() = 0;
		virtual void Shutdown() = 0;
	};
}
