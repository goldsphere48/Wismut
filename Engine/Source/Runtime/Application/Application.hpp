#pragma once
#include "Core/Defines.hpp"
#include "Window.hpp"

namespace Wi
{
	class Application
	{
	public:
		void Run();

	private:
		void Shutdown();

	private:
		bool m_IsRunning = true;
		SharedPtr<Window> m_MainWindow;
	};
}
