#pragma once
#include "LayerStack.h"
#include "Window.h"

namespace Wi
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
		void PushOverlay(Layer* layer) { m_LayerStack.PushOverlay(layer); }

	private:
		void ProcessEvents() const;

	private:
		LayerStack m_LayerStack;
		std::shared_ptr<Window> m_Window;
		bool m_Running = true;
	};
}
