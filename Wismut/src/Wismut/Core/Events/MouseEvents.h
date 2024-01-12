#pragma once
#include "Event.h"
#include "Wismut/Core/KeyCodes.h"
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
		MouseMovedEvent(float mouseX, float mouseY)
			: m_MouseX(mouseX), m_MouseY(mouseY)
		{

		}

		EVENT_TYPE(MouseMoved)

		float GetMouseX() const { return m_MouseX; }
		float GetMouseY() const { return m_MouseY; }

	private:
		float m_MouseX;
		float m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset)
		{

		}

		EVENT_TYPE(MouseScrolled)

		float GetOffsetX() const { return m_xOffset; }
		float GetOffsetY() const { return m_yOffset; }

	private:
		float m_xOffset;
		float m_yOffset;
	};
}