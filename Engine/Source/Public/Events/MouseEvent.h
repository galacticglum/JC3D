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

class MouseMovedEvent : public Event
{
public:
	explicit MouseMovedEvent(const Vector2f position) : Position(position)
	{
	}

	const Vector2f Position;

	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "MouseMovedEvent: " << Position.X << ", " << Position.Y;
		return stream.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const Vector2f offset) : Offset(offset)
	{
	}

	const Vector2f Offset;

	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "MouseScrolledEvent: " << Offset.X << ", " << Offset.Y;
		return stream.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
};

class MouseButtonEvent : public Event
{
public:
	int GetMouseButton() const
	{
		return m_Button;
	}

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
protected:
	explicit MouseButtonEvent(const int button) : m_Button(button) {}

	int m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	explicit MouseButtonPressedEvent(const int button) : MouseButtonEvent(button) {}
	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "MouseButtonPressedEvent: " << m_Button;
		return stream.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	explicit MouseButtonReleasedEvent(const int button) : MouseButtonEvent(button) {}
	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};
