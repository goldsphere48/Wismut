#pragma once
#include "Layer.h"

#include <vector>

namespace Wi
{
	class LayerStack
	{
	public:
		void Destroy();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_Stack.begin(); }
		std::vector<Layer*>::iterator end() { return m_Stack.end(); }

	private:
		std::vector<Layer*> m_Stack;
		int m_LayerIndex = 0;
	};
}
