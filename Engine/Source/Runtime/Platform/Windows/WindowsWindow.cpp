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

		m_WindowHandle = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_TRANSPARENT,
			AppWindowClass,
			title,
			WS_OVERLAPPEDWINDOW,
			definition.PositionX,
			definition.PositionY,
			definition.Width,
			definition.Height,
			nullptr,
			nullptr,
			instance,
			nullptr
		);

		m_Width = definition.Width;
		m_Height = definition.Height;

		ShowWindow(m_WindowHandle, SW_SHOW);
	}

	void WindowsWindow::Destroy()
	{
		if (m_WindowHandle)
		{
			DestroyWindow(m_WindowHandle);
		}
	}

	unsigned WindowsWindow::GetWidth()
	{
		return m_Width;
	}

	unsigned WindowsWindow::GetHeight()
	{
		return m_Height;
	}
}
#endif
