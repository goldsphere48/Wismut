#pragma once
#include "INativeApplication.hpp"
#include "Core/Defines.hpp"
#include "Window.hpp"

namespace Wi
{
	class Application
	{
	public:
		void Run();

	private:
		void Shutdown() const;

	private:
		bool m_IsRunning = true;
		SharedPtr<Window> m_MainWindow;
		UniquePtr<INativeApplication> m_NativeApplication;
	};
}
