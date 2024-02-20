#include "wipch.h"
#include "Application.h"

#include "Input.h"
#include "Logger.h"
#include "Wismut/Renderer/Renderer.h"

namespace Wi
{
	Application::Application(const ApplicationConfig& config) : m_ApplicationConfig(config)
	{
		Logger::Initialize();

		WI_CORE_ASSERT(!s_Instance, "Attempt to create application twice");

		s_Instance = this;

		WI_CORE_INFO("Initializing application...");

		Input::Initialize();

		const auto windowSpecification = WindowSpecification
		{
			.Title = config.ApplicationName,
			.Width = config.WindowWidth,
			.Height = config.WindowHeight,
			.EventCallback = [this](Event& e) { return this->OnEvent(e); }
		};

		m_Window = Window::Create(windowSpecification);

		Renderer::Initialize(m_ApplicationConfig.RendererConfig);
	}

	void Application::Run()
	{
		while(m_Running)
		{
			ProcessEvents();

			Renderer::Begin();

			for (const auto layer : m_LayerStack)
				layer->Update();

			Renderer::End();

			m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % Renderer::GetConfig().MaxFramesInFlight;

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
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return this->OnResizeEvent(e); });

		return true;
	}

	bool Application::OnCloseEvent(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnResizeEvent(const WindowResizeEvent& event)
	{
		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
		return true;
	}

	void Application::OnShutdown()
	{
		m_Window->Destroy();
		Renderer::Shutdown();
		m_LayerStack.Destroy();
	}
}
