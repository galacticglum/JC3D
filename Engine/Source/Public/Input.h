/**
 * Author: Shon Verch
 * File Name: Input.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: A platform-independent input interface.
 */

#pragma once

#include <Math/Vector.h>

/**
 * @class Input Input.h
 * @brief A platform-independent input interface.
 */
class Input
{
public:
	virtual ~Input() = default;

	/**
	 * @brief Indicates whether the key with the specified @p keycode is pressed.
	 */
	static bool IsKeyPressed(const int keycode)
	{
		return s_Instance->IsKeyPressed_Impl(keycode);
	}

	/**
	 * @brief Indicates whether the mouse button with the specified @p button code is pressed.
	 */
	static bool IsMouseButtonPressed(const int button)
	{
		return s_Instance->IsMouseButtonPressed_Impl(button);
	}

	/**
	 * @brief Gets the position of the mouse button with the specified @p button code.
	 */
	static Vector2f GetMousePosition()
	{
		return s_Instance->GetMousePosition_Impl();
	}
protected:
	/**
	 * @brief Platform specific implementation of Input::IsKeyPressed.
	 */
	virtual bool IsKeyPressed_Impl(int keycode) = 0;

	/**
	 * @brief Platform specific implementation of Input::IsMouseButtonPressed.
	 */
	virtual bool IsMouseButtonPressed_Impl(int button) = 0;

	/**
	 * @brief Platform specific implementation of Input::GetMousePosition.
	 */
	virtual Vector2f GetMousePosition_Impl() = 0;
private:
	static Input* s_Instance;
};