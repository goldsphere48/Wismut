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
		bool OnMouseScrolled(float get_z_delta);

	protected:
		bool OnEvent(Event& event);
		bool OnWindowClose();

	private:
		void Shutdown() const;

	private:
		bool m_IsRunning = true;
		SharedPtr<IPlatformWindow> m_MainWindow;
		UniquePtr<IPlatformApplication> m_NativeApplication;
	};
}
