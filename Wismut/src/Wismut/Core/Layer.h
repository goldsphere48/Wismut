#pragma once

namespace Wi
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		void virtual OnAttach() {}
		void virtual OnDetach() {}
		void virtual Update() {}
	};
}
