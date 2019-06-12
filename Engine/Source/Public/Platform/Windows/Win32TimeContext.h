/**
 * Author: Shon Verch
 * File Name: Win32TimeContext.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: Win32 implementation of the TimeContext class.
 */

#pragma once

#include <TimeContext.h>

/**
 * @class Win32TimeContext Win32TimeContext.h
 * @brief Win32 implementation of the TimeContext class.
 */
class Win32TimeContext final : public TimeContext
{
public:
	/**
	 * @brief Gets the time, in seconds, from the start of the application.
	 */
	double GetTime() override;
};
