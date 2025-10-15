#pragma once
#ifdef WI_PLATFORM_WIN

#include <Windows.h>
#include "Platform/IPlatformWindow.h"
#include "Renderer/IRenderViewport.h"

namespace Wi
{
	class WindowsWindow : public IPlatformWindow
	{
	public:
		explicit WindowsWindow(const WindowDefinition& definition, HINSTANCE instance);
		void Destroy() override;
		uint16 GetWidth() override;
		uint16 GetHeight() override;
		void* GetNativeHandle() const override;
		IRenderViewport* GetViewport() override;

		static const wchar_t* AppWindowClass;

	private:
		IRenderViewport* m_Viewport = nullptr;
		HWND m_WindowHandle = nullptr;
		int m_Width;
		int m_Height;
	};
}
#endif
