/**
 * Author: Shon Verch
 * File Name: Win32Input.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description:  The Win32 implementation of the Input class.
 */

#pragma once

#include <Input.h>

/**
 * @class Win32Input Win32Input.h
 * @brief The Win32 implementation of the Input class.
 */
class Win32Input final : public Input
{
protected:
	/**
	 * @brief The Win32 specific implementation of Input::IsKeyPressed.
	 */
	bool IsKeyPressed_Impl(int keycode) override;

	/**
	 * @brief The Win32 specific implementation of Input::IsMouseButtonPressed.
	 */
	bool IsMouseButtonPressed_Impl(int button) override;

	/**
	 * @brief The Win32 specific implementation of Input::GetMousePosition.
	 */
	Vector2f GetMousePosition_Impl() override;
};