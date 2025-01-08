#ifdef WI_PLATFORM_WIN
#include "Application/Windows/WindowsWindow.hpp"
#include "Platform/Platform.hpp"

namespace Wi
{
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	struct PlatformState
	{
		HINSTANCE HInstance;
		HICON Icon;
		HCURSOR Cursor;
	} *GState;

	bool Platform::Startup()
	{
		if (GState)
		{
			return false;
		}

		GState = new PlatformState;
		GState->HInstance = GetModuleHandle(nullptr);
		GState->Icon = LoadIcon(GState->HInstance, IDI_APPLICATION);
		GState->Cursor = LoadCursor(GState->HInstance, IDC_ARROW);

		const WNDCLASS wc = {
			.style = CS_DBLCLKS,
			.lpfnWndProc = WndProc,
			.cbClsExtra = 0,
			.cbWndExtra = 0,
			.hInstance = GState->HInstance,
			.hIcon = GState->Icon,
			.hCursor = GState->Cursor,
			.hbrBackground = static_cast<HBRUSH>(GetStockObject(COLOR_WINDOW + 1)),
			.lpszMenuName = nullptr,
			.lpszClassName = WindowsWindow::AppWindowClass
		};

		RegisterClass(&wc);

		return true;
	}

	void Platform::Shutdown()
	{
		delete GState;
	}

	SharedPtr<Window> Platform::MakeWindow(const WindowDefinition& config)
	{
		return CreateSharedPtr<WindowsWindow>(config, GState->HInstance);
	}

	void* Platform::GetNativeState()
	{
		return GState;
	}
}
#endif
