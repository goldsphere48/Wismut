#ifdef WI_PLATFORM_WIN
#include "WindowsWindow.hpp"
#include "Platform/Platform.hpp"

namespace Wi
{
	const wchar_t* WindowsWindow::AppWindowClass = L"WismutWindow";

	WindowsWindow::WindowsWindow(const WindowDefinition& definition, HINSTANCE instance)
	{
		WCHAR title[256];
		MultiByteToWideChar(CP_UTF8, 0, definition.Title.c_str(), -1, title, 256);

		m_Hwnd = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_TRANSPARENT,
			AppWindowClass,
			title,
			WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION,
			definition.PositionX,
			definition.PositionY,
			definition.Width,
			definition.Height,
			nullptr,
			nullptr,
			instance,
			nullptr
		);

		ShowWindow(m_Hwnd, SW_SHOW);
	}

	void WindowsWindow::Destroy()
	{
		if (m_Hwnd)
		{
			DestroyWindow(m_Hwnd);
		}
	}
}
#endif
