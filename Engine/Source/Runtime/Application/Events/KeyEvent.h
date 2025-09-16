#pragma once
#include "Event.h"
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

		KeyCode GetKey() const
		{
			return Key;
		}

	protected:
		KeyCode Key;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keyCode, int repeatedCount)
			: KeyEvent(keyCode), m_repeatedCount(repeatedCount)
		{
		}

		EVENT_TYPE(KeyPressed)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressed: " << Key << " " << m_repeatedCount;
			return ss.str();
		}

		int GetRepeatedCount() const
		{
			return m_repeatedCount;
		}

		bool IsRepeated() const
		{
			return m_repeatedCount > 1;
		}

	private:
		int m_repeatedCount;
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
