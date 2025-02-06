#pragma once
#include <string>

#include "Core.h"

namespace Wi
{
	struct WindowDefinition
	{
		std::string Title;
		i32 PositionX;
		i32 PositionY;
		i32 Width;
		i32 Height;
	};

	class IPlatformWindow
	{
	public:
		RULE_OF_FIVE(IPlatformWindow)
		virtual ~IPlatformWindow() = default;

		virtual i32 GetWidth() = 0;
		virtual i32 GetHeight() = 0;
		virtual void Destroy() = 0;
	};
}
