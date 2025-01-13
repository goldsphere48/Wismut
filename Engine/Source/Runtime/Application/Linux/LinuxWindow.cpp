#ifdef WI_PLATFORM_LINUX
#include "LinuxWindow.hpp"

namespace Wi
{
	LinuxWindow::LinuxWindow(const WindowDefinition& definition, xcb_connection_t* connection)
		: m_Connection(connection)
	{
		m_HWnd = xcb_generate_id(connection);
		m_Screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

		xcb_create_window(
			connection,
			XCB_COPY_FROM_PARENT,
			m_HWnd,
			m_Screen->root,
			definition.PositionX,
			definition.PositionY,
			definition.Width,
			definition.Height,
			1,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			m_Screen->root_visual,
			0,
			nullptr
		);

		xcb_map_window(connection, m_HWnd);
		xcb_flush(connection);
	}

	void LinuxWindow::Destroy()
	{
		xcb_destroy_window(m_Connection, m_HWnd);
	}
}
#endif
