#pragma once
#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>
#include "Platform/IPlatformWindow.hpp"

namespace Wi
{
	class LinuxApplication;

	class LinuxWindow final : public IPlatformWindow
	{
	public:
		explicit LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection);
		void Destroy() override;
		unsigned GetWidth() override;
		unsigned GetHeight() override;

	private:
		xcb_connection_t* m_Connection;
		xcb_window_t m_WindowHandle;
		xcb_screen_t* m_Screen;

		int m_Width = 0;
		int m_Height = 0;

		friend LinuxApplication;
	};
}
#endif
