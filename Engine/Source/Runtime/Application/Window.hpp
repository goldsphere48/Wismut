#pragma once
#include <string>

namespace Wi
{
	struct WindowDefinition
	{
		std::string Title;
		int PositionX;
		int PositionY;
		int Width;
		int Height;
	};

	class Window
	{
	public:
		virtual ~Window() = default;
		Window() = default;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		virtual void PumpMessages() = 0;
		virtual void Destroy() = 0;
	};
}
