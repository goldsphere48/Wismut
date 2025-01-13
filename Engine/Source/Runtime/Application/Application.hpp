#pragma once
#include "Core/Defines.hpp"
#include "Platform/IPlatformApplication.hpp"
#include "Platform/IPlatformWindow.hpp"

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
		SharedPtr<IPlatformWindow> m_MainWindow;
		UniquePtr<IPlatformApplication> m_NativeApplication;
	};
}
