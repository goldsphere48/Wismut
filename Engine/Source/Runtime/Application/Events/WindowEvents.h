#pragma once
#include "Event.hpp"
#include <sstream>

namespace Wi
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_TYPE(WindowClose)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_Width(width), m_Height(height)
		{

		}

		EVENT_TYPE(WindowResize)

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Resize: " << m_Width << " " << m_Height;
			return ss.str();
		}

	private:
		int m_Width;
		int m_Height;
	};
}
