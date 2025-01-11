#ifdef WI_PLATFORM_LINUX
#include <xcb/xcb.h>
#include "Application/Linux/LinuxWindow.hpp"
#include "Platform/Platform.hpp"

namespace Wi
{
	struct PlatformState
	{
		xcb_connection_t* Connection = nullptr;
	} *GState;

	bool Platform::Startup()
	{
		if (GState)
		{
			return false;
		}

		GState = new PlatformState;
		GState->Connection = xcb_connect(nullptr, nullptr);

		if (!GState->Connection)
		{
			return false;
		}

		return true;
	}

	void Platform::Shutdown()
	{
		xcb_disconnect(GState->Connection);
		delete GState;
		GState = nullptr;
	}

	SharedPtr<Window> Platform::MakeWindow(const WindowDefinition& config)
	{
		return CreateSharedPtr<LinuxWindow>(config, GState->Connection);
	}

	void* Platform::GetNativeState()
	{
		return GState;
	}
}

#endif
