#include "Application.hpp"

#include "Platform/Platform.hpp"

namespace Wi
{
	void Application::Run()
	{
		if (!Platform::Startup())
		{
			return;
		}

		const WindowDefinition windowConfig = WindowDefinition{
			.Title = "Wismut Engine",
			.PositionX = 300,
			.PositionY = 300,
			.Width = 720,
			.Height = 480
		};

		m_MainWindow = Platform::MakeWindow(windowConfig);

		while (m_IsRunning)
		{
			m_MainWindow->PumpMessages();
		}

		Platform::Shutdown();
	}

	void Application::Shutdown()
	{
		m_MainWindow->Destroy();
	}
}
