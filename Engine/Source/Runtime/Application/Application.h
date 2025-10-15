#pragma once

#include "Application/Events/Event.h"
#include "Platform/IPlatformApplication.h"
#include "Platform/IPlatformWindow.h"

namespace Wi
{
	class Application
	{
	public:
		virtual ~Application() = default;

		void Run();
		bool HandleEvent(Event& event);

		static Application* GetInstance() { return s_Instance; }

	protected:
		bool OnWindowClose();

		virtual void OnInit() {}
		virtual void OnUpdate() {}
		virtual void OnRender(SharedPtr<IPlatformWindow> window) {}
		virtual void OnEvent(Event& event) {};

	private:
		void Update();
		void Render();
		void Shutdown() const;

	private:
		static Application* s_Instance;
		UniquePtr<IPlatformApplication> m_NativeApplication;
		SharedPtr<IPlatformWindow> m_MainWindow;
		bool m_IsRunning = true;
	};

	Application* CreateApplication();
}
