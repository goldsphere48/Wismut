#pragma once
#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>
#include "Application/Window.hpp"

namespace Wi
{
	class LinuxWindow final : public Window
	{
	public:
		explicit LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection);

		void PumpMessages() override;
		void Destroy() override;

	private:
		xcb_window_t m_HWnd;
		xcb_screen_t* m_Screen;
	};
}
#endif
