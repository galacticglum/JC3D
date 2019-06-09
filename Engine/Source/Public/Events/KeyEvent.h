/**
 * Author: Shon Verch
 * File Name: KeyEvent.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: Keyboard related events.
 */

#pragma once

#include <sstream>
#include <Events/Event.h>

class KeyEvent : public Event
{
public:
	int GetKeyCode() const { return m_KeyCode; }
	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
	explicit KeyEvent(const int keycode) : m_KeyCode(keycode)
	{
	}

	int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(const int keycode, const int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount)
	{
	}

	int GetRepeatCount() const
	{
		return m_RepeatCount;
	}

	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
		return stream.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
private:
	int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	explicit KeyReleasedEvent(const int keycode) : KeyEvent(keycode)
	{
	}

	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "KeyReleasedEvent: " << m_KeyCode;
		return stream.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	explicit KeyTypedEvent(const int keycode) : KeyEvent(keycode)
	{
	}

	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "KeyTypedEvent: " << m_KeyCode;
		return stream.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};