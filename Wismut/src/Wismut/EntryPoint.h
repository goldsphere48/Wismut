#pragma once

#include "Core/Application.h"

Wi::Application* CreateApplication();

int main()
{
	Wi::Application* app = CreateApplication();
	app->Run();
	delete app;
}