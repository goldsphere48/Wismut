#pragma once
#ifdef WI_PLATFORM_WIN
#include <Windows.h>

#include "Platform/IPlatformApplication.h"

namespace Wi
{
	class WindowsApplication final : public IPlatformApplication
	{
	public:
		void Startup() override;
		void Shutdown() override;
		SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) override;
		void PumpMessages() override;

		static UniquePtr<IPlatformApplication> CreateApplication();

	private:
		HINSTANCE m_Instance = nullptr;
		HICON m_Icon = nullptr;
		HCURSOR m_Cursor = nullptr;
	};

	using PlatformApplication = WindowsApplication;
}
#endif
