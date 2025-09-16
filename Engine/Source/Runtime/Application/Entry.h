#pragma once
#include <Application/Application.h>

#include "Core/Utils/CompilerMacros.h"

namespace Wi
{
	FORCEINLINE int RunEngine()
	{
		Application* app = CreateApplication();
		app->Run();
		delete app;
		return 0;
	}
}
