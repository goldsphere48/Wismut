#include "LinuxWindow.hpp"
#ifdef WI_PLATFORM_LINUX
#include "LinuxApplication.hpp"

namespace Wi
{
	bool LinuxApplication::Startup()
	{
		m_Connection = xcb_connect(nullptr, nullptr);

		if (!m_Connection)
		{
			return false;
		}

		return true;
	}

	SharedPtr<Window> LinuxApplication::MakeWindow(const WindowDefinition& config)
	{
		return CreateSharedPtr<LinuxWindow>(config, m_Connection);
	}

	void LinuxApplication::PumpMessages()
	{
		xcb_generic_event_t* event;
		while ((event = xcb_wait_for_event(m_Connection)))
		{
			switch (event->response_type & 0x80)
			{
				default:
					break;
			}

			free(event);
		}
	}

	void LinuxApplication::Shutdown()
	{
		xcb_disconnect(m_Connection);
	}
}

#endif
