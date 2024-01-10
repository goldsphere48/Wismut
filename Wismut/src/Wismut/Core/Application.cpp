#include "wipch.h"
#include "Application.h"

#include "Logger.h"

namespace Wi
{
	Application::Application()
	{
		Logger::Initialize();
		WI_CORE_INFO("Initializing application...")
	}

	Application::~Application()
	{
		m_LayerStack.Destroy();
	}

	void Application::Run()
	{
		while(m_Running)
		{
			for (const auto layer : m_LayerStack)
				layer->Update();
		}
	}
}
