#ifdef WI_PLATFORM_WIN

#include "WindowsWindow.h"

#include <array>

#include "Core/Assertion.h"
#include "Platform/Platform.h"
#include "Core/Logger/Logger.h"
#include "Renderer/Renderer.h"

namespace Wi
{
	const wchar_t* WindowsWindow::AppWindowClass = L"WismutWindow";

	WindowsWindow::WindowsWindow(const WindowDefinition& definition, HINSTANCE instance)
		: m_Width(definition.Width), m_Height(definition.Height)
	{
		Log::Info("Creating Windows window...");

		constexpr int titleSize = 256;
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

		m_Viewport = Renderer::GetBackend()->CreateViewport(m_WindowHandle);

		WI_ASSERT(m_WindowHandle)

		ShowWindow(m_WindowHandle, SW_SHOW);
	}

	void WindowsWindow::Destroy()
	{
		if (m_WindowHandle != nullptr)
		{
			Renderer::GetBackend()->DestroyViewport(m_Viewport);
			DestroyWindow(m_WindowHandle);
		}
	}

	uint16 WindowsWindow::GetWidth()
	{
		return static_cast<uint16>(m_Width);
	}

	uint16 WindowsWindow::GetHeight()
	{
		return static_cast<uint16>(m_Height);
	}

	void* WindowsWindow::GetNativeHandle() const
	{
		return m_WindowHandle;
	}

	IRenderViewport* WindowsWindow::GetViewport()
	{
		return m_Viewport;
	}
}
#endif
