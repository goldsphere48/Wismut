#pragma once
#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsPlatform.h"

Wi::Application* CreateApplication();

inline void Main()
{
	Wi::GenericApplication* app = CreateApplication();
	app->Run();
	delete app;
}

#ifndef WI_DISTRIBUTION
// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
int main()
{
	Main();
}
#else
#ifdef WI_PLATFORM_WIN
#include <windows.h>
// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
int32 APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Main();
}
#else
#error "Unsupported platform"
#endif
#endif
