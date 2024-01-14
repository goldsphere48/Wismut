#include "wipch.h"
#include "Application.h"

#include "Input.h"
#include "Logger.h"
#include "Wismut/Renderer/Renderer.h"

namespace Wi
{
	Application::Application()
	{
		Logger::Initialize();

		WI_CORE_ASSERT(!s_Instance, "Attempt to create application twice");

		s_Instance = this;

		WI_CORE_INFO("Initializing application...");

		Input::Initialize();

		const auto windowSpecification = WindowSpecification
		{
			.Title = "Wismut App",
			.Width = 1280,
			.Height = 720,
			.EventCallback = [this](Event& e) { return this->OnEvent(e); }
		};

		m_Window = Window::Create(windowSpecification);

		Renderer::Initialize();
	}

	void Application::Run()
	{
		while(m_Running)
		{
			ProcessEvents();

			for (const auto layer : m_LayerStack)
				layer->Update();

			Input::ClearReleasedKeys();
		}

		OnShutdown();
	}

	void Application::ProcessEvents() const
	{
		Input::TransitionPressedKeys();
		Input::TransitionPressedButtons();

		m_Window->PollEvents();
	}

	bool Application::OnEvent(Event& event)
	{
		for (const auto layer : m_LayerStack)
			layer->OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->OnCloseEvent(e); });

		return true;
	}

	bool Application::OnCloseEvent(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	void Application::OnShutdown()
	{
		m_Window->Destroy();
		Renderer::Shutdown();
		m_LayerStack.Destroy();
	}
}
