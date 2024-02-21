#include "wipch.h"
#include "LayerStack.h"

namespace Wi
{
	void LayerStack::Destroy()
	{
		for (Layer* const layer : m_Stack)
			layer->OnDetach();

		m_Stack.clear();
		m_LayerIndex = 0;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Stack.insert(m_Stack.begin() + m_LayerIndex++, layer);
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		m_Stack.push_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto it = std::ranges::find(m_Stack, layer);
		if (it != m_Stack.end()) {
			m_Stack.erase(it);
			layer->OnDetach();
			m_LayerIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		const auto it = std::ranges::find(m_Stack, layer);
		if (it != m_Stack.end()) {
			m_Stack.erase(it);
			layer->OnDetach();
		}
	}
}
