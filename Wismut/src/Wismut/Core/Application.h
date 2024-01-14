#pragma once
#include "LayerStack.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/WindowEvents.h"

namespace Wi
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
		void PushOverlay(Layer* layer) { m_LayerStack.PushOverlay(layer); }

		const std::shared_ptr<Window>& GetWindow() { return m_Window; }

		static Application* Get() { return s_Instance; }

	private:
		void ProcessEvents() const;
		bool OnEvent(Event& event);
		bool OnCloseEvent(WindowCloseEvent& event);
		void OnShutdown();

	private:
		inline static Application* s_Instance;
		LayerStack m_LayerStack;
		std::shared_ptr<Window> m_Window;
		bool m_Running = true;
	};
}
