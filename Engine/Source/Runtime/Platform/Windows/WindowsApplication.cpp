#ifdef WI_PLATFORM_WIN
#include "WindowsApplication.hpp"

namespace Wi
{
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	bool WindowsApplication::Startup()
	{
		m_Instance = GetModuleHandle(nullptr);
		m_Icon = LoadIcon(m_Instance, IDI_APPLICATION);
		m_Cursor = LoadCursor(m_Instance, IDC_ARROW);

		const WNDCLASS wc = {
			.style = CS_DBLCLKS,
			.lpfnWndProc = WndProc,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = m_Instance,
			.hIcon = m_Icon,
			.hCursor = m_Cursor,
			.hbrBackground = static_cast<HBRUSH>(GetStockObject(COLOR_WINDOW + 1)),
			.lpszMenuName = nullptr,
			.lpszClassName = WindowsWindow::AppWindowClass
		};

		RegisterClass(&wc);

		return true;
	}

	void WindowsApplication::Shutdown()
	{
		
	}

	SharedPtr<IPlatformWindow> WindowsApplication::MakeWindow(const WindowDefinition& config)
	{
		return CreateSharedPtr<WindowsWindow>(config, m_Instance);
	}

	void WindowsApplication::PumpMessages()
	{
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
#endif
