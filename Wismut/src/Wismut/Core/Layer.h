#pragma once
#include "Events/Event.h"

namespace Wi
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual bool OnEvent(Event& event) { return false; }
		virtual void Update() {}
	};
}
