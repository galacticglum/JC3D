/**
 * Author: Shon Verch
 * File Name: Event.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: The base class for all events.
 */
	
#pragma once

#include <Core.h>
#include <string>
#include <functional>

/**
 * @enum EventType Event.h
 * @brief The type of event.
 */
enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

/**
 * @enum EventCategory Event.h
 * @brief The type of event category.
 */
enum EventCategory
{
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4)
};

/**
 * @brief Macro for event class types.
 * @note Utility macro to simplify the creation of new event classes based on the EventType enum.
 */
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; } \
							   virtual EventType GetEventType() const override { return GetStaticType(); } \
							   virtual std::string GetName() const override { return #type; } 

/**
 * @brief Macro for event categories.
 */
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

/**
 * @class Event Event.h
 * @brief The base class for all events.
 */
class Event
{
public:
	/**
	 * @brief Indicates whether this Event is handled.
	 */
	bool Handled = false;

	/**
	 * @brief Gets the type of this Event.
	 */
	virtual EventType GetEventType() const = 0;

	/**
	 * @brief Gets the name of this Event.
	 */
	virtual std::string GetName() const = 0;

	/**
	 * @brief Gets the category flags for this Event.
	 */
	virtual int GetCategoryFlags() const = 0;

	/**
	 * @brief Gets the string representation of this Event.
	 */
	virtual std::string ToString() const { return GetName(); }

	/**
	 * @brief Indicates whether this Event belongs to the specified @p category.
	 */
	bool IsInCategory(const EventCategory category) const
	{
		return GetCategoryFlags() & category;
	}
};

/**
 * @class EventDispather Event.h
 * @brief Responsible for dispatching events.
 */
class EventDispather
{
	template<typename T>
	using EventHandler = std::function<bool(T&)>;
public:
	/**
	 * @brief Initializes a new EventDispatcher for the specified @p event.
	 */
	explicit EventDispather(Event& event) : m_Event(event)
	{
	}

	template<typename T>
	bool Dispatch(EventHandler<T> handler)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.Handled = handler(*static_cast<T*>(&m_Event));
			return true;
		}

		return false;
	}
private:
	Event& m_Event;
};

/**
 * @brief Stream operator overload for Event.
 */
inline std::ostream& operator <<(std::ostream& stream, const Event& right)
{
	return stream << right.ToString();
}