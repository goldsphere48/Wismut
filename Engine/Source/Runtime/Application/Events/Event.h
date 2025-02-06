#pragma once
#include <functional>
#include <string>

namespace Wi
{
	enum class EventType
	{
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

#define EVENT_TYPE(type) const char* GetName() const override { return #type; }\
						 EventType GetType() const override { return EventType::type; }\
						 static EventType GetStaticType() { return EventType::type; }

	class Event
	{
	public:
		virtual ~Event() = default;
		virtual const char* GetName() const = 0;
		virtual EventType GetType() const = 0;
		virtual std::string ToString() const { return GetName(); }
		bool Handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : m_Event(event)
		{

		}

		template<typename T>
		bool Dispatch(const std::function<bool(T&)>& handler)
		{
			if (T::GetStaticType() == m_Event.GetType() && !m_Event.Handled)
			{
				m_Event.Handled |= handler(static_cast<T&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	using EventCallback = std::function<bool(Event&)>;
}
