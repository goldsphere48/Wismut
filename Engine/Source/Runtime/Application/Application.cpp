#include "Application.hpp"

#include "Platform/Platform.hpp"

namespace Wi
{
	void Application::Run()
	{
		m_NativeApplication = Platform::CreateNativeApplication();
		if (!m_NativeApplication->Startup())
		{
			return;
		}

		const WindowDefinition windowConfig = WindowDefinition {
			.Title = "Wismut Engine",
			.PositionX = 300,
			.PositionY = 300,
			.Width = 720,
			.Height = 480
		};

		m_MainWindow = m_NativeApplication->MakeWindow(windowConfig);

		while (m_IsRunning)
		{
			m_NativeApplication->PumpMessages();
		}

		Shutdown();
	}

	void Application::Shutdown() const
	{
		m_MainWindow->Destroy();
		m_NativeApplication->Shutdown();
	}
}
