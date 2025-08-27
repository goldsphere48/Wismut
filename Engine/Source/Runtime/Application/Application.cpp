#include "Application.h"

#include "Events/WindowEvents.h"
#include "Platform/Platform.h"
#include "Core.h"

namespace Wi
{
	Application* Application::s_Instance = nullptr;

	void Application::Run()
	{
		CORE_CHECK(!s_Instance)

		s_Instance = this;

		Log::Info("Wismut Engine Initialization...");

		m_NativeApplication = PlatformApplication::CreateApplication();

		CORE_CHECK(m_NativeApplication->Startup())

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

	bool Application::OnEvent(Event& event)
	{
		Event::Dispatch<WindowCloseEvent>(event, [this](const WindowCloseEvent&) { return this->OnWindowClose(); });
		return true;
	}

	bool Application::OnWindowClose()
	{
		m_IsRunning = false;
		return true;
	}

	void Application::Shutdown() const
	{
		m_MainWindow->Destroy();
		m_NativeApplication->Shutdown();
	}
}
