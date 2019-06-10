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

class Win32Input : public Input
{
protected:
	bool IsKeyPressed_Impl(int keycode) override;
	bool IsMouseButtonPressed_Impl(int button) override;
	Vector2f GetMousePosition_Impl() override;
};