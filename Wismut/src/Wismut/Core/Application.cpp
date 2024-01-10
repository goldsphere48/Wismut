#include "wipch.h"
#include "Application.h"

#include "Logger.h"

namespace Wi
{
	Application::Application()
	{
		Logger::Initialize();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WI_CORE_INFO("Starting application...")
	}
}
