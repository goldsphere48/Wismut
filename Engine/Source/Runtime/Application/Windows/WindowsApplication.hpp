#pragma once
#include <vector>
#ifdef WI_PLATFORM_WIN
#include "WindowsWindow.hpp"
#include "Application/INativeApplication.hpp"

namespace Wi
{
	class WindowsApplication final : public INativeApplication
	{
	public:
		bool Startup() override;
		void Shutdown() override;
		SharedPtr<Window> MakeWindow(const WindowDefinition& config) override;
		void PumpMessages() override;

	private:
		HINSTANCE m_Instance = nullptr;
		HICON m_Icon = nullptr;
		HCURSOR m_Cursor = nullptr;
	};
}
#endif
