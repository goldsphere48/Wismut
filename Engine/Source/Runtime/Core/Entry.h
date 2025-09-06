#pragma once
#include <Application/Application.h>

#include "Memory/Memory.h"

Wi::Application* CreateApplication();

inline int Main()
{
	Wi::Memory::Initialize();
	Wi::Application* app = CreateApplication();
	app->Run();
	delete app;
	Wi::Memory::Shutdown();
	return 0;
}

int main()
{
	Main();
}
