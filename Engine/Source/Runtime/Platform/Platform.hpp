#pragma once
#include "Application/Window.hpp"
#include "Core/Defines.hpp"

namespace Wi
{
	class Platform
	{
	public:
		static bool Startup();
		static void Shutdown();
		static SharedPtr<Window> MakeWindow(const WindowDefinition& config);
		static void* GetNativeState();
	};
}
