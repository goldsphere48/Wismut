#pragma once
#include <string>

#include "Core.h"

namespace Wi
{
	struct WindowDefinition
	{
		std::string Title;
		i16 PositionX;
		i16 PositionY;
		u16 Width;
		u16 Height;
	};

	class IPlatformWindow
	{
	public:
		DELETE_COPYING(IPlatformWindow)
		virtual ~IPlatformWindow() = default;

		virtual u16 GetWidth() = 0;
		virtual u16 GetHeight() = 0;
		virtual void Destroy() = 0;
	};
}
