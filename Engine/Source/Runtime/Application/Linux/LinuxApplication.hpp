#pragma once
#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>
#include "Application/INativeApplication.hpp"

namespace Wi
{
	class LinuxApplication final : public INativeApplication
	{
	public:
		bool Startup() override;
		SharedPtr<Window> MakeWindow(const WindowDefinition& config) override;
		void PumpMessages() override;
		void Shutdown() override;
	private:
		xcb_connection_t* m_Connection = nullptr;
	};
}
#endif
