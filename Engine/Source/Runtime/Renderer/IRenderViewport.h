#pragma once

namespace Wi
{
	struct IRenderViewport
	{
		virtual ~IRenderViewport() = default;

		virtual void MakeCurrent() = 0;
		virtual void Present() = 0;
	};
}
