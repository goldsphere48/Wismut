#pragma once

#include "Core.h"

namespace Wi
{
	class WI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}


