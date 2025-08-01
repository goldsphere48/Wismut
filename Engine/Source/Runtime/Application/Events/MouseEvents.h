#pragma once
#include "Event.h"
#include "Application/KeyCodes.h"
#include <sstream>

namespace Wi
{
	class MouseButtonEvent : public Event
	{
	public:
		MouseButton GetMouseButton() const { return Button; }

	protected:
		MouseButtonEvent(MouseButton mouseButton)
			: Button(mouseButton)
		{

		}

	protected:
		MouseButton Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton mouseButton)
			: MouseButtonEvent(mouseButton)
		{

		}

		EVENT_TYPE(MouseButtonPressed)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MousePressedEvent: " << Button;
			return ss.str();
		}
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton mouseButton)
			: MouseButtonEvent(mouseButton)
		{

		}

		EVENT_TYPE(MouseButtonReleased)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleasedEvent: " << Button;
			return ss.str();
		}
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int mouseX, int mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY)
		{

		}

		EVENT_TYPE(MouseMoved)

		int GetMouseX() const { return m_MouseX; }
		int GetMouseY() const { return m_MouseY; }

	private:
		int m_MouseX;
		int m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float zDelta)
			: m_zDelta(zDelta)
		{

		}

		EVENT_TYPE(MouseScrolled)

		float32 GetZDelta() const { return m_zDelta; }

	private:
		float32 m_zDelta;
	};
}
