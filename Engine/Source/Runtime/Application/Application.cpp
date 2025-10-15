#include "Application.h"

#include "Core/Assertion.h"
#include "Core/Logger/Logger.h"
#include "Core/Memory/Memory.h"
#include "Events/WindowEvents.h"
#include "Platform/Platform.h"
#include "Input.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvents.h"
#include "Renderer/Renderer.h"

namespace Wi
{
	static bool OnKeyPressedEvent(const KeyPressedEvent& event);
	static bool OnKeyReleasedEvent(const KeyReleasedEvent& event);
	static bool OnMousePressedEvent(const MouseButtonPressedEvent& event);
	static bool OnMouseReleasedEvent(const MouseButtonReleasedEvent& event);
	static bool OnMouseMovedEvent(const MouseMovedEvent& event);
	static bool OnMouseScrollEvent(const MouseScrolledEvent& event);

	Application* Application::s_Instance = nullptr;

	void Application::Run()
	{
		WI_ASSERT(!s_Instance)

		s_Instance = this;

		Memory::GetInstance()->Initialize();

		Log::Info("Wismut Engine Initialization...");

		Input::Initialize();

		m_NativeApplication = PlatformApplication::CreateApplication();
		m_NativeApplication->Startup();

		Renderer::Initialize();

		const WindowDefinition windowConfig = WindowDefinition {
			.Title = "Wismut Engine",
			.PositionX = 300,
			.PositionY = 300,
			.Width = 720,
			.Height = 480
		};

		m_MainWindow = m_NativeApplication->MakeWindow(windowConfig);

		OnInit();

		while (m_IsRunning)
		{
			Input::TransitionPressedKeys();
			Input::TransitionPressedButtons();
			m_NativeApplication->PumpMessages();

			Update();
			Render();

			Input::ClearReleasedKeys();
		}

		Shutdown();
	}

	void Application::Update()
	{
		OnUpdate();
	}

	void Application::Render()
	{
		auto* viewport = m_MainWindow->GetViewport();
		Renderer::GetBackend()->BeginRenderViewport(viewport);
		Renderer::GetBackend()->ClearColor(0, 1, 0);
		Renderer::GetBackend()->EndRenderViewport(viewport);
		OnRender(m_MainWindow);
	}

	bool Application::HandleEvent(Event& event)
	{
		Event::Dispatch<WindowCloseEvent>(event, [this](const WindowCloseEvent&) { return this->OnWindowClose(); });
		Event::Dispatch<KeyPressedEvent>(event, [](const KeyPressedEvent& e) { return OnKeyPressedEvent(e); });
		Event::Dispatch<KeyReleasedEvent>(event, [](const KeyReleasedEvent& e) { return OnKeyReleasedEvent(e); });
		Event::Dispatch<MouseButtonPressedEvent>(event, [](const MouseButtonPressedEvent& e) { return OnMousePressedEvent(e); });
		Event::Dispatch<MouseButtonReleasedEvent>(event, [](const MouseButtonReleasedEvent& e) { return OnMouseReleasedEvent(e); });
		Event::Dispatch<MouseMovedEvent>(event, [](const MouseMovedEvent& e) { return OnMouseMovedEvent(e); });
		Event::Dispatch<MouseScrolledEvent>(event, [](const MouseScrolledEvent& e) { return OnMouseScrollEvent(e); });

		OnEvent(event);

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
		Renderer::Shutdown();
		m_NativeApplication->Shutdown();
		Memory::GetInstance()->Shutdown();
	}

	static bool OnKeyPressedEvent(const KeyPressedEvent& event)
	{
		Input::UpdateKeyState(event.GetKey(), event.IsRepeated() ? KeyState::Held : KeyState::Pressed);
		return false;
	}

	static bool OnKeyReleasedEvent(const KeyReleasedEvent& event)
	{
		Input::UpdateKeyState(event.GetKey(), KeyState::Released);
		return false;
	}

	static bool OnMousePressedEvent(const MouseButtonPressedEvent& event)
	{
		Input::UpdateMouseButtonState(event.GetMouseButton(), KeyState::Pressed);
		return false;
	}

	static bool OnMouseReleasedEvent(const MouseButtonReleasedEvent& event)
	{
		Input::UpdateMouseButtonState(event.GetMouseButton(), KeyState::Released);
		return false;
	}

	static bool OnMouseMovedEvent(const MouseMovedEvent& event)
	{
		Input::UpdateMousePosition(event.GetMouseX(), event.GetMouseY());
		return false;
	}

	static bool OnMouseScrollEvent(const MouseScrolledEvent& event)
	{
		Input::UpdateMouseScrollDelta(event.GetZDelta());
		return false;
	}
}
