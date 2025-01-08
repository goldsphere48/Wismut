#pragma once
#ifdef WI_PLATFORM_WIN

#include <Windows.h>
#include "Application/Window.hpp"

namespace Wi
{
	class WindowsWindow : public Window
	{
	public:
		explicit WindowsWindow(const WindowDefinition& definition, HINSTANCE instance);

		void PumpMessages() override;
		void Destroy() override;

	public:
		static const wchar_t* AppWindowClass;

	private:
		HWND m_Hwnd = nullptr;
	};
}
#endif
