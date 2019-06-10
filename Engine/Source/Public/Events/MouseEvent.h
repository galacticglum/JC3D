/**
 * Author: Shon Verch
 * File Name: MouseEvent.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: Mouse related events.
 */

#pragma once

#include <Events/Event.h>
#include <Math/Vector.h>
#include <sstream>

/**
 * @class MouseMovedEvent MouseEvent.h
 * @brief Raised when the mouse cursor moves.
 */
class MouseMovedEvent : public Event
{
public:
	/**
	 * @brief The new position of the mouse.
	 */
	const Vector2f Position;

	/**
	 * @brief Initialize a new MouseMovedEvent given its new @p position.
	 */
	explicit MouseMovedEvent(const Vector2f position) : Position(position)
	{
	}

	/**
	 * @brief Convert this MouseEvent to its string representation.
	 */
	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "MouseMovedEvent: " << Position.X << ", " << Position.Y;
		return stream.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

/**
 * @class MouseScrolledEvent MouseEvent.h
 * @brief Raised when the user scrolls via the mouse.
 */
class MouseScrolledEvent : public Event
{
public:
	/**
	 * @brief The scroll offsets.
	 */
	const Vector2f Offset;

	/**
	 * @brief Initialize a new MouseScrolledEvent given the scroll @offset.
	 */
	explicit MouseScrolledEvent(const Vector2f offset) : Offset(offset)
	{
	}

	/**
	 * @brif Convert this MouseScrolledEvent to its string representation.
	 */
	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "MouseScrolledEvent: " << Offset.X << ", " << Offset.Y;
		return stream.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

/**
 * @class MouseButtonEvent MouseEvent.h
 * @brief The base Event class for all events related to mouse buttons.
 */
class MouseButtonEvent : public Event
{
public:
	/**
	 * @brief Gets the mouse button pertaining to this MouseButtonEvent.
	 */
	int GetMouseButton() const
	{
		return m_Button;
	}

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
	/**
	 * @brief Initialize a new MouseButtonEvent given the @p button.
	 */
	explicit MouseButtonEvent(const int button) : m_Button(button)
	{
	}

	/**
	 * @brief The mouse button pertaining to this MouseButtonEvent.
	 */
	int m_Button;
};

/**
 * @class MouseButtonPressedEvent MouseEvent.h
 * @brief Raised when a mouse button is pressed.
 */
class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	/**
	 * @brief Initialize a new MouseButtonPressedEvent given the @p button.
	 */
	explicit MouseButtonPressedEvent(const int button) : MouseButtonEvent(button)
	{
	}

	/**
	 * @brief Converts this MouseButtonPressedEvent to its string representation.
	 */
	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "MouseButtonPressedEvent: " << m_Button;
		return stream.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

/**
 * @class MouseButtonReleasedEvent MouseEvent.h
 * @brief Raised when a mouse button is released.
 */
class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	/**
	 * @brief Initialize a new MouseButtonReleasedEvent given the @p button.
	 */
	explicit MouseButtonReleasedEvent(const int button) : MouseButtonEvent(button)
	{
	}

	/**
	 * @brief Converts this MouseButtonReleasedEvent to its string representation.
	 */
	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};
