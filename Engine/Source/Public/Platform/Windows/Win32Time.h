/**
 * Author: Shon Verch
 * File Name: Win32Time.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: Win32 implementation of the Time class.
 */

#pragma once

#include <Time.h>

/**
 * @class Win32Time Win32Time.h
 * @brief Win32 implementation of the Time class.
 */
class Win32Time final : public Time
{
public:
	/**
	 * @brief Gets the time, in seconds, from the start of the application.
	 */
	double GetTime() override;
};
