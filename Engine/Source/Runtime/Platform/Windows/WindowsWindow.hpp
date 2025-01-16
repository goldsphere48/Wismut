#pragma once
#ifdef WI_PLATFORM_WIN

#include <Windows.h>
#include "Platform/IPlatformWindow.hpp"

namespace Wi
{
	class WindowsWindow : public IPlatformWindow
	{
	public:
		explicit WindowsWindow(const WindowDefinition& definition, HINSTANCE instance);
		void Destroy() override;
		i32 GetWidth() override;
		i32 GetHeight() override;

		static const wchar_t* AppWindowClass;

	private:
		HWND m_WindowHandle = nullptr;
		i32 m_Width;
		i32 m_Height;
	};
}
#endif
