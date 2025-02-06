#include "Application.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvents.h"
#include "Events/WindowEvents.h"
#include "Platform/Platform.h"
#include "Core/Assertion.h"

namespace Wi
{
	Application* Application::s_Instance = nullptr;

	void Application::Run()
	{
		CORE_CHECK(!s_Instance)

		s_Instance = this;
		m_NativeApplication = Platform::CreateNativeApplication();

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
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>([this](const WindowCloseEvent&) { return this->OnWindowClose(); });
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
