#pragma once
#include "Application.h"
#include "Log.h"

#ifdef WI_PLATFORM_WINDOWS

extern Wi::Application* Wi::CreateApplication();

int main(int argc, char** argv)
{
	Wi::Log::Init();
	WI_CORE_INFO("Initialize Engine");
	Wi::Application* app = Wi::CreateApplication();
	app->Run();
	delete app;
}

#endif
