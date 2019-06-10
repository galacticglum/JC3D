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
#include <Input.h>

/**
 *@class KeyEvent KeyEvent.h
 * @brief The base Event class for all keyboard related events.
 */
class KeyEvent : public Event
{
public:
	/**
	 * @brief Get the keycode pertaining to this KeyEvent.
	 */
	KeyCode GetKeyCode() const
	{
		return m_KeyCode;
	}

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	/**
	 * @brief Initialize a new KeyEvent given a @p keycode.
	 */
	explicit KeyEvent(const KeyCode keycode) : m_KeyCode(keycode)
	{
	}

	/**
	 * @brief The keycode pertaining to this KeyEvent.
	 */
	KeyCode m_KeyCode;
};

/**
 * @class KeyPressedEvent KeyEvent.h
 * @brief Raised when a key is pressed.
 */
class KeyPressedEvent : public KeyEvent
{
public:
	/**
	 * @brief Initialize a new KeyPressedEvent given the @p keycode and @p repeatCount.
	 * @param keycode The code of the key that was pressed.
	 * @param repeatCount The number of times the key was repeated.
	 */
	KeyPressedEvent(const KeyCode keycode, const int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount)
	{
	}

	/**
	 * @brief Get the repeat count of the key.
	 */
	int GetRepeatCount() const
	{
		return m_RepeatCount;
	}

	/**
	 * @brief Convert this KeyPressedEvent to its string representation.
	 */
	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "KeyPressedEvent: " << static_cast<int>(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
		return stream.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
private:
	/**
	 * @brief The number of times the key was repeated.
	 */
	int m_RepeatCount;
};

/**
 * @class KeyReleasedEvent KeyEvent.h
 * @brief Raised when a key is released.
 */
class KeyReleasedEvent : public KeyEvent
{
public:
	/**
	 * @brief Initialize a new KeyReleasedEvent given the @p keycode.
	 */
	explicit KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode)
	{
	}

	/**
	 * @brief Convert this KeyReleasedEvent to its string representation.
	 */
	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "KeyReleasedEvent: " << static_cast<int>(m_KeyCode);
		return stream.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

/**
 * @class KeyTypedEvent KeyEvent.h
 * @brief Raised when a key is typed.
 * @note While this class is structurally identical to KeyEvent and
 *		 similar to KeyPressedEvent, it is functionally very different.
 *		 Unlike key input, key typing obeys keyboard layout and modifiers keys.
 * @note Furthermore, the purpose of this event is fundamentally different
 *		 to KeyPressedEvent and as such, they should not be used interchangeably.
 *		 This event should be used for text input while KeyPressedEvent should be used
 *		 for keyboard input.
 */
class KeyTypedEvent : public KeyEvent
{
public:
	/**
	 * @brief Initialize a new KeyTypedEvent given the @p keycode.
	 */
	explicit KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode)
	{
	}

	/**
	 * @Brief Convert this KeyTypedEvent to its string representation.
	 */
	std::string ToString() const override
	{
		std::stringstream stream;
		stream << "KeyTypedEvent: " << static_cast<int>(m_KeyCode);
		return stream.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};