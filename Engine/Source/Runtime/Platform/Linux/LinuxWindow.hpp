#pragma once
#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>
#include "Platform/IPlatformWindow.hpp"

namespace Wi
{
	class LinuxWindow final : public IPlatformWindow
	{
	public:
		explicit LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection);
		void Destroy() override;

	private:
		xcb_connection_t* m_Connection;
		xcb_window_t m_HWnd;
		xcb_screen_t* m_Screen;
	};
}
#endif
