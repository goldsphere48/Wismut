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
		unsigned GetWidth() override;
		unsigned GetHeight() override;

		static const wchar_t* AppWindowClass;

	private:
		HWND m_WindowHandle = nullptr;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}
#endif
