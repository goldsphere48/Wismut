#ifdef WI_PLATFORM_WIN
#include "WindowsWindow.h"
#include "Platform/Platform.h"
#include <array>

namespace Wi
{
	const wchar_t* WindowsWindow::AppWindowClass = L"WismutWindow";

	WindowsWindow::WindowsWindow(const WindowDefinition& definition, HINSTANCE instance)
		: m_Width(definition.Width), m_Height(definition.Height)
	{
		constexpr i32 titleSize = 256;
		std::array<WCHAR, titleSize> title = { };
		MultiByteToWideChar(CP_UTF8, 0, definition.Title.c_str(), -1, title.data(), titleSize);

		m_WindowHandle = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_TRANSPARENT,
			AppWindowClass,
			title.data(),
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

		ShowWindow(m_WindowHandle, SW_SHOW);
	}

	void WindowsWindow::Destroy()
	{
		if (m_WindowHandle != nullptr)
		{
			DestroyWindow(m_WindowHandle);
		}
	}

	i32 WindowsWindow::GetWidth()
	{
		return m_Width;
	}

	i32 WindowsWindow::GetHeight()
	{
		return m_Height;
	}
}
#endif
