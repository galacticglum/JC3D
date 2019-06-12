/**
 * Author: Shon Verch
 * File Name: TimeContext.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: Platform independent timing utilities.
 */

#pragma once

/**
 * @class TimeContext TimeContext.h
 * @brief Platform independent timing utilities.
 */
class TimeContext
{
public:
	virtual ~TimeContext() = default;
	/**
	 * @brief Get the time, in seconds, from the start of the application.
	 */
	virtual double GetTime() = 0;

	/**
	 * @brief Create a TimeContext context.
	 */
	static TimeContext* Create();
};