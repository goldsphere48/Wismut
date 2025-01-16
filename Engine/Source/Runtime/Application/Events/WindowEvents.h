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
		WindowResizeEvent(i32 width, i32 height)
			: m_Width(width), m_Height(height)
		{

		}

		EVENT_TYPE(WindowResize)

		i32 GetWidth() const { return m_Width; }
		i32 GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Resize: " << m_Width << " " << m_Height;
			return ss.str();
		}

	private:
		i32 m_Width;
		i32 m_Height;
	};
}
