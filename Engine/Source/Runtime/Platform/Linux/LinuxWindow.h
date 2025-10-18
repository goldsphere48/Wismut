#pragma once
#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>

#include "Platform/IPlatformWindow.h"
#include "Renderer/IRenderViewport.h"

namespace Wi
{
	class LinuxApplication;

	class LinuxWindow final : public IPlatformWindow
	{
	public:
		explicit LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection);
		void Destroy() override;
		uint16 GetWidth() override;
		uint16 GetHeight() override;
		void* GetNativeHandle() const override;
		IRenderViewport* GetViewport() override;

	private:
		xcb_connection_t* m_Connection;
		xcb_screen_t* m_Screen;
		xcb_window_t m_WindowHandle;

		uint16 m_Width = 0;
		uint16 m_Height = 0;

		IRenderViewport* m_Viewport;

		friend LinuxApplication;
	};
}
#endif
