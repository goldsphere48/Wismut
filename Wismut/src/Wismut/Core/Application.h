#pragma once
#include "LayerStack.h"

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
		LayerStack m_LayerStack;
		bool m_Running = true;
	};
}
