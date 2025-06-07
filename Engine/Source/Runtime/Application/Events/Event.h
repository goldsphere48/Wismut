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

		template<typename T>
		static bool Dispatch(Event& event, const std::function<bool(T&)>& handler)
		{
			if (T::GetStaticType() == event.GetType() && !event.Handled)
			{
				event.Handled |= handler(static_cast<T&>(event));
				return true;
			}

			return false;
		}
	};

	using EventCallback = std::function<bool(Event&)>;
}
