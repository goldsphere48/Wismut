#pragma once
#include "Core/Defines.hpp"
#include "Application/Events/Event.hpp"
#include "Platform/IPlatformApplication.hpp"
#include "Platform/IPlatformWindow.hpp"

namespace Wi
{
	class Application
	{
	public:
		void Run();
		bool OnEvent(Event& event);

		static Application* InstancePtr() { return s_Instance; }

	protected:
		bool OnWindowClose();

	private:
		void Shutdown() const;

	private:
		static Application* s_Instance;
		bool m_IsRunning = true;
		SharedPtr<IPlatformWindow> m_MainWindow;
		UniquePtr<IPlatformApplication> m_NativeApplication;
	};
}
