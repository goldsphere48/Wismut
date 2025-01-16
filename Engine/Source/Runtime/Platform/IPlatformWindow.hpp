#pragma once
#include <string>

#include "Core/Defines.hpp"

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
		virtual ~IPlatformWindow() = default;
		IPlatformWindow() = default;
		IPlatformWindow(const IPlatformWindow&) = delete;
		IPlatformWindow(IPlatformWindow&&) = delete;
		IPlatformWindow& operator=(const IPlatformWindow&) = delete;
		IPlatformWindow& operator=(IPlatformWindow&&) = delete;

		virtual i32 GetWidth() = 0;
		virtual i32 GetHeight() = 0;
		virtual void Destroy() = 0;
	};
}
