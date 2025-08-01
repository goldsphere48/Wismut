#pragma once
#ifdef WI_PLATFORM_WIN

#include <Windows.h>
#include "Platform/IPlatformWindow.h"

namespace Wi
{
	class WindowsWindow : public IPlatformWindow
	{
	public:
		explicit WindowsWindow(const WindowDefinition& definition, HINSTANCE instance);
		void Destroy() override;
		uint16 GetWidth() override;
		uint16 GetHeight() override;

		static const wchar_t* AppWindowClass;

	private:
		HWND m_WindowHandle = nullptr;
		int m_Width;
		int m_Height;
	};
}
#endif
