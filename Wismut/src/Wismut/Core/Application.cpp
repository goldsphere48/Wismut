#include "wipch.h"
#include "Application.h"

#include "Logger.h"

namespace Wi
{
	Application::Application()
	{
		Logger::Initialize();
		WI_CORE_INFO("Initializing application...")

		const auto windowSpecification = WindowSpecification
		{
			.Title = "Wismut App",
			.Width = 1280,
			.Height = 720,
		};

		m_Window = Window::Create(windowSpecification);
	}

	Application::~Application()
	{
		m_LayerStack.Destroy();
	}

	void Application::Run()
	{
		while(m_Running)
		{
			ProcessEvents();

			for (const auto layer : m_LayerStack)
				layer->Update();
		}
	}

	void Application::ProcessEvents() const
	{
		m_Window->PollEvents();
	}
}
