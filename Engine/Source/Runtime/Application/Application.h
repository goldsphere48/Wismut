#pragma once

#include "Application/Events/Event.h"
#include "Core/Memory/Memory.h"
#include "Platform/IPlatformApplication.h"
#include "Platform/IPlatformWindow.h"

namespace Wi
{
	class Application : public UseSystemMalloc
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
		UniquePtr<IPlatformApplication> m_NativeApplication;
		SharedPtr<IPlatformWindow> m_MainWindow;
		bool m_IsRunning = true;
	};
}
