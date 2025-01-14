#pragma once
#ifdef WI_PLATFORM_LINUX
#include <map>
#include <xcb/xcb.h>
#include "LinuxWindow.hpp"
#include "Platform/IPlatformApplication.hpp"
#include "Platform/IPlatformWindow.hpp"

namespace Wi
{
	class LinuxApplication final : public IPlatformApplication
	{
	public:
		bool Startup() override;
		SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) override;
		void PumpMessages() override;
		void Shutdown() override;
		void SetEventCallback(const EventCallback& eventCallback) override;

	private:
		static xcb_atom_t s_AtomWmDeleteWindow;
		static xcb_atom_t s_AtomWmProtocols;

		xcb_connection_t* m_Connection = nullptr;
		std::map<xcb_window_t, SharedPtr<IPlatformWindow>> m_Windows;

		friend LinuxWindow;
	};
}
#endif
