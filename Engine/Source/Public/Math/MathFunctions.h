/**
 * Author: Shon Verch
 * File Name: MathFunctions.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/06/19
 * Modified Date: 06/06/19
 * Description: Collection of useful maths functions.
 */

#pragma once

#include <iostream>
#include <type_traits>
#include <Logger.h>

/**
 * @brief Macro definition for the generic matrix template typename definition.
 * @note Restricts to arithmetic types only.
 */
#define MATH_FUNCTION_TEMPLATE template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>

/**
 * @struct MathFunctions MathFunctions.h "Public/Math/MathFunctions.h"
 * @brief Collection of useful maths functions.
 */
struct MathFunctions
{
	static constexpr double PI = 3.14159265358;
	static constexpr double PI_OVER_2 = PI / 2;
	static constexpr double PI_OVER_3 = PI / 3;
	static constexpr double PI_OVER_4 = PI / 4;
	static constexpr double PI_OVER_6 = PI / 6;
	static constexpr double TWO_PI = 2 * PI;
	static constexpr double THREE_PI_OVER_2 = 3 * PI / 2;
	static constexpr double E = 2.71828182845904523536;
	static constexpr double LOG_10E = 0.434294482;
	static constexpr double LOG_2E = 1.442695041;

	static const float Epsilon;
	static const long MaxLong;
	static const int MaxInt;
	static const float MaxFloat;
	static const double MaxDouble;

	/**
	 * @brief Gets the next power of two from @p n.
	 */
	MATH_FUNCTION_TEMPLATE
	static T NextPowerOfTwo(const T& n)
	{
		if (n < 0)
		{
			Logger::Log("Engine", LoggerVerbosity::Warning, "MathFunctions::NextPowerOfTwo: 'n' must be positive!");
			exit(1);
		}

		return static_cast<T>(std::pow(2, std::ceil(std::log2(static_cast<double>(n)))));
	}

	/**
	 * @brief Gets the factorial of @p n.
	 */
	static long Factorial(int n);

	/**
	 * @brief Gets the value of @p n choose @k.
	 */
	static long BinomialCoefficient(int n, int k);

	/**
	 * @brief Finds the inverse square root of @p x.
	 * @note Perform an inverse square root using Chris Lomont's fast method.
	 *		 Based on Lomont's paper: "Fast Inverse Square Root" (@link http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf). 
	 * @param x The value of the radicand.
	 * @returns The value of the inverse square root of @p x.
	 */
	static float InverseSqrtFast(const float x)
	{
		return static_cast<float>(InverseSqrtFast(x));
	}

	/**
	 * @brief Finds the inverse square root of @p x.
	 * @note Perform an inverse square root using Chris Lomont's fast method.
	 *		 Based on Lomont's paper: "Fast Inverse Square Root" (@link http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf).
	 * @param x The value of the radicand.
	 * @returns The value of the inverse square root of @p x.
	 */
	static double InverseSqrtFast(double x);

	/**
	 * @brief Converts @p degrees to radians.
	 */
	MATH_FUNCTION_TEMPLATE
	static T DegreesToRadians(const T& degrees)
	{
		const T degreeToRadian = static_cast<T>(PI / 180);
		return degrees * degreeToRadian;
	}

	/**
	 * @brief Converts @p radians to degrees.
	 */
	MATH_FUNCTION_TEMPLATE
	static T RadiansToDegree(const T& radians)
	{
		const T radianToDegree = static_cast<T>(180 / PI);
		return radians * radianToDegree;
	}

	/**
	 * @brief Swaps @p a and @p b.
	 */
	MATH_FUNCTION_TEMPLATE
	static void Swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	/**
	 * @brief Linearly interpolates between @p a and @b by the interpolant @p t.
	 */
	MATH_FUNCTION_TEMPLATE
	static T Lerp(const T& a, const T& b, const T& t)
	{
		return a + (b - a) * t;
	}

	/**
	 * @brief Determines whether @p left and @p right are approximately equal.
	 * @note To be approximately equals mean that the difference of the two
	 *		 values is less than MathFunctions::Epsilon.
	 */
	MATH_FUNCTION_TEMPLATE
	static bool IsEqualApproximate(const T& left, const T& right)
	{
		return std::abs(left - right) < Epsilon;
	}
};