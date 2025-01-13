#pragma once
#include "Window.hpp"
#include "Core/Defines.hpp"

namespace Wi
{
	class INativeApplication
	{
	public:
		virtual ~INativeApplication() = default;
		virtual bool Startup() = 0;
		virtual SharedPtr<Window> MakeWindow(const WindowDefinition& config) = 0;
		virtual void PumpMessages() = 0;
		virtual void Shutdown() = 0;
	};
}
