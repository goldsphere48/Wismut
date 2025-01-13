#pragma once
#include "Event.hpp"
#include "Application/KeyCodes.h"
#include <sstream>

namespace Wi
{
	class KeyEvent : public Event
	{
	public:
		KeyEvent(KeyCode keyCode)
			: Key(keyCode)
		{

		}

		KeyCode GetKey() const { return Key; }

	protected:
		KeyCode Key;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keyCode, bool repeated)
			: KeyEvent(keyCode), m_IsRepeated(repeated)
		{
		}

		EVENT_TYPE(KeyPressed)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressed: " << Key << " " << m_IsRepeated;
			return ss.str();
		}

		bool IsRepeated() const { return m_IsRepeated; }

	private:
		bool m_IsRepeated;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode)
			: KeyEvent(keyCode)
		{
		}

		EVENT_TYPE(KeyReleased)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleased: " << Key;
			return ss.str();
		}
	};
}
