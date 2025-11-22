#ifdef WI_PLATFORM_LINUX
#include "LinuxWindow.h"

#include "LinuxApplication.h"
#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"
#include "Renderer/Renderer.h"

namespace Wi
{
	LinuxWindow::LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection)
		: m_Connection(connection)
	{
		Log::Info("Creating Xcb window...");

		m_WindowHandle = xcb_generate_id(connection);
		WI_ASSERT(m_WindowHandle)

		m_Screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
		WI_ASSERT(m_Screen)

		uint32 eventMask =
			XCB_EVENT_MASK_EXPOSURE |
			XCB_EVENT_MASK_STRUCTURE_NOTIFY |
			XCB_EVENT_MASK_POINTER_MOTION |
			XCB_EVENT_MASK_BUTTON_PRESS |
			XCB_EVENT_MASK_BUTTON_RELEASE |
			XCB_EVENT_MASK_KEY_PRESS |
			XCB_EVENT_MASK_KEY_RELEASE;

		uint32 valueList[] = { m_Screen->black_pixel, eventMask };
		int16 clientX = definition.PositionX;
		int16 clientY = definition.PositionY;
		uint16 clientWidth =  definition.Width;
		uint16 clientHeight = definition.Height;

		auto request = xcb_create_window_checked(
			connection,
			XCB_COPY_FROM_PARENT,
			m_WindowHandle,
			m_Screen->root,
			clientX,
			clientY,
			clientWidth,
			clientHeight,
			1,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			m_Screen->root_visual,
			XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK,
			valueList
		);
		auto error = xcb_request_check(m_Connection, request);
		WI_ASSERT(!error)

		request = xcb_change_property_checked(
			connection,
			XCB_PROP_MODE_REPLACE,
			m_WindowHandle,
			LinuxApplication::s_AtomWmProtocols,
			4,
			32,
			1,
			&LinuxApplication::s_AtomWmDeleteWindow
		);
		error = xcb_request_check(m_Connection, request);
		WI_ASSERT(!error)

		request = xcb_map_window_checked(connection, m_WindowHandle);
		error = xcb_request_check(m_Connection, request);
		WI_ASSERT(!error)

		xcb_flush(connection);

		m_Width = definition.Width;
		m_Height = definition.Height;

		xcb_window_t* windowPtr = new xcb_window_t(m_WindowHandle);
		m_Viewport = Renderer::GetBackend()->CreateViewport(windowPtr);
		delete windowPtr;
	}

	void LinuxWindow::Destroy()
	{
		Renderer::GetBackend()->DestroyViewport(m_Viewport);
		xcb_destroy_window(m_Connection, m_WindowHandle);
	}

	uint16 LinuxWindow::GetWidth()
	{
		return m_Width;
	}

	uint16 LinuxWindow::GetHeight()
	{
		return m_Height;
	}

	void* LinuxWindow::GetNativeHandle() const
	{
		return reinterpret_cast<void*>(m_WindowHandle);
	}

	IRenderViewport* LinuxWindow::GetViewport()
	{
		return m_Viewport;
	}
}
#endif
