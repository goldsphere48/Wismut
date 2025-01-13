#pragma once
#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>
#include "Platform/IPlatformApplication.hpp"
#include "Platform/IPlatformWindow.hpp"

namespace Wi
{
	class LinuxApplication final : public IPlatformApplication
	{
	public:
		bool Startup() override;
		SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) override;
		void PumpMessages() override;
		void Shutdown() override;
	private:
		xcb_connection_t* m_Connection = nullptr;
	};
}
#endif
