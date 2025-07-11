#ifdef WI_PLATFORM_LINUX
#include "LinuxWindow.h"

#include <limits>

#include "Core.h"
#include "LinuxApplication.h"

namespace Wi
{
	LinuxWindow::LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection)
		: m_Connection(connection)
	{
		Log::Info("Creating Xcb window...");

		m_WindowHandle = xcb_generate_id(connection);
		CORE_CHECK(m_WindowHandle)

		m_Screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
		CORE_CHECK(m_Screen)

		u32 eventMask =
			XCB_EVENT_MASK_EXPOSURE |
			XCB_EVENT_MASK_STRUCTURE_NOTIFY |
			XCB_EVENT_MASK_POINTER_MOTION |
			XCB_EVENT_MASK_BUTTON_PRESS |
			XCB_EVENT_MASK_BUTTON_RELEASE |
			XCB_EVENT_MASK_KEY_PRESS |
			XCB_EVENT_MASK_KEY_RELEASE;

		u32 valueList[] = { m_Screen->black_pixel, eventMask };
		i16 clientX = definition.PositionX;
		i16 clientY = definition.PositionY;
		u16 clientWidth =  definition.Width;
		u16 clientHeight = definition.Height;

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
		CORE_CHECK(!error)

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
		CORE_CHECK(!error)

		request = xcb_map_window_checked(connection, m_WindowHandle);
		error = xcb_request_check(m_Connection, request);
		CORE_CHECK(!error)

		xcb_flush(connection);

		m_Width = definition.Width;
		m_Height = definition.Height;
	}

	void LinuxWindow::Destroy()
	{
		xcb_destroy_window(m_Connection, m_WindowHandle);
	}

	u16 LinuxWindow::GetWidth()
	{
		return m_Width;
	}

	u16 LinuxWindow::GetHeight()
	{
		return m_Height;
	}
}
#endif
