#pragma once
#ifdef WI_PLATFORM_WIN
#include "WindowsWindow.hpp"
#include "Platform/IPlatformApplication.hpp"

namespace Wi
{
	class WindowsApplication final : public IPlatformApplication
	{
	public:
		bool Startup() override;
		void Shutdown() override;
		SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) override;
		void PumpMessages() override;

	private:
		HINSTANCE m_Instance = nullptr;
		HICON m_Icon = nullptr;
		HCURSOR m_Cursor = nullptr;
	};
}
#endif
