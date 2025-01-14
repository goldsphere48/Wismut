#ifdef WI_PLATFORM_LINUX
#include "LinuxWindow.hpp"
#include "LinuxApplication.hpp"

namespace Wi
{
	LinuxWindow::LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection)
		: m_Connection(connection)
	{
		m_WindowHandle = xcb_generate_id(connection);
		m_Screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

		uint32_t eventMask =
			XCB_EVENT_MASK_EXPOSURE |
			XCB_EVENT_MASK_STRUCTURE_NOTIFY |
			XCB_EVENT_MASK_POINTER_MOTION |
			XCB_EVENT_MASK_BUTTON_PRESS |
			XCB_EVENT_MASK_BUTTON_RELEASE |
			XCB_EVENT_MASK_KEY_PRESS |
			XCB_EVENT_MASK_KEY_RELEASE;

		uint32_t valueList[] = { m_Screen->black_pixel, eventMask };

		int16_t clientX = static_cast<int16_t>(definition.PositionX);
		int16_t clientY = static_cast<int16_t>(definition.PositionY);
		uint16_t clientWidth = static_cast<uint16_t>(definition.Width);
		uint16_t clientHeight = static_cast<uint16_t>(definition.Height);

		xcb_create_window(
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

		xcb_change_property(
			connection,
			XCB_PROP_MODE_REPLACE,
			m_WindowHandle,
			LinuxApplication::s_AtomWmProtocols,
			4,
			32,
			1,
			&LinuxApplication::s_AtomWmDeleteWindow
		);

		xcb_map_window(connection, m_WindowHandle);
		xcb_flush(connection);

		m_Width = definition.Width;
		m_Height = definition.Height;
	}

	void LinuxWindow::Destroy()
	{
		xcb_destroy_window(m_Connection, m_WindowHandle);
	}

	unsigned LinuxWindow::GetWidth()
	{
		return m_Width;
	}

	unsigned LinuxWindow::GetHeight()
	{
		return m_Height;
	}
}
#endif
