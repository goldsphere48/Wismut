#pragma once
#include "Core.h"
#include "Application/Events/Event.h"
#include "Core/Logger/Logger.h"
#include "Platform/IPlatformApplication.h"
#include "Platform/IPlatformWindow.h"

namespace Wi
{
	class Application
	{
	public:
		void Run();
		bool OnEvent(Event& event);

		static Application* InstancePtr() { return s_Instance; }
		const Logger* GetCoreLoggerPtr() const { return m_CoreLogger; }

	protected:
		bool OnWindowClose();

	private:
		void Shutdown() const;

	private:
		static Application* s_Instance;
		bool m_IsRunning = true;
		Logger* m_CoreLogger = nullptr;
		SharedPtr<IPlatformWindow> m_MainWindow;
		UniquePtr<IPlatformApplication> m_NativeApplication;
	};
}
