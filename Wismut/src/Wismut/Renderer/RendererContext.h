#pragma once

namespace Wi
{
	class RendererContext
	{
	public:
		virtual void Initialize() = 0;
		virtual void Destroy() = 0;
	};
}

