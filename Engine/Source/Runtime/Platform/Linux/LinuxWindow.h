#pragma once
#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>
#include "Platform/IPlatformWindow.h"

namespace Wi
{
	class LinuxApplication;

	class LinuxWindow final : public IPlatformWindow
	{
	public:
		explicit LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection);
		void Destroy() override;
		u16 GetWidth() override;
		u16 GetHeight() override;

	private:
		xcb_connection_t* m_Connection;
		xcb_screen_t* m_Screen;
		xcb_window_t m_WindowHandle;

		u16 m_Width = 0;
		u16 m_Height = 0;

		friend LinuxApplication;
	};
}
#endif
