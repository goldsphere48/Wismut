#ifdef WI_PLATFORM_LINUX
#include "LinuxWindow.h"

#include <limits>

#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"
#include "LinuxApplication.h"

namespace Wi
{
	LinuxWindow::LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection)
		: m_Connection(connection)
	{
		WI_CORE_INFO("Creating Xcb window...")

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

		CORE_CHECK(definition.PositionX <= std::numeric_limits<i16>::max());
		CORE_CHECK(definition.PositionY <= std::numeric_limits<i16>::max());
		CORE_CHECK(definition.Width >=0 && definition.Width <= std::numeric_limits<u16>::max());
		CORE_CHECK(definition.Height >= 0 && definition.Height <= std::numeric_limits<u16>::max());

		i16 clientX = static_cast<i16>(definition.PositionX);
		i16 clientY = static_cast<i16>(definition.PositionY);
		u16 clientWidth = static_cast<u16>(definition.Width);
		u16 clientHeight = static_cast<u16>(definition.Height);

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

	i32 LinuxWindow::GetWidth()
	{
		return m_Width;
	}

	i32 LinuxWindow::GetHeight()
	{
		return m_Height;
	}
}
#endif
