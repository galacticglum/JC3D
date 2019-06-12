/**
 * Author: Shon Verch
 * File Name: Time.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: Platform independent timing utilities.
 */

#pragma once

/**
 * @class Time Time.h
 * @brief Platform independent timing utilities.
 */
class Time
{
public:
	virtual ~Time() = default;
	/**
	 * @brief Get the time, in seconds, from the start of the application.
	 */
	virtual double GetTime() = 0;

	/**
	 * @brief Create a Time context.
	 */
	static Time* Create();
};