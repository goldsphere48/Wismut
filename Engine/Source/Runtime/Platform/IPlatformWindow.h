#pragma once
#include <string>

#include "Core.h"

namespace Wi
{
	struct WindowDefinition
	{
		std::string Title;
		int16 PositionX;
		int16 PositionY;
		uint16 Width;
		uint16 Height;
	};

	class IPlatformWindow
	{
	public:
		DELETE_COPYING(IPlatformWindow)
		virtual ~IPlatformWindow() = default;

		virtual uint16 GetWidth() = 0;
		virtual uint16 GetHeight() = 0;
		virtual void Destroy() = 0;
	};
}
