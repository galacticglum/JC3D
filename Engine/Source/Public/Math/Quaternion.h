/**
 * Author: Shon Verch
 * File Name: Quaternion.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/07/2019
 * Modified Date: 06/07/2019
 * Description: A 4-dimensional vector consisting of a scalar part w and a 3-dimensional semi-rotation axis v.
 */

#pragma once

#include <Math/Vector.h>

/**
 * @struct Quaternion Quaternion.h
 * @brief A four-dimensional vector consisting of a scalar part w and a 3-dimensional semi-rotation axis v.
 */
struct Quaternion
{
	/**
	 * @brief The x-component of the vector part of this Quaternion.
	 */
	float X;

	/**
	 * @brief The y-component of the vector part of this Quaternion.
	 */
	float Y;

	/**
	 * @brief The z-component of the vector part of this Quaternion.
	 */
	float Z;

	/**
	 * @brief The scalar part of this Quaternion.
	 */
	float W;

	/**
	 * @brief Initializes a new Quaternion as the identity quaternion.
	 */
	Quaternion() : X(0), Y(0), Z(0), W(1)
	{
	}

	/**
	 * @brief Initializes a new Quaternion given the components.
	 * @param x The x-component of this Quaternion.
	 * @param y The y-component of this Quaternion.
	 * @param z The z-component of this Quaternion.
	 * @param w The w-component of this Quaternion.
	 */
	Quaternion(const float x, const float y, const float z, const float w) : X(x), Y(y), Z(z), W(w)
	{
	}

	/**
	 * @brief Initializes a new Quaternion given the @p vector and @p scalar part.
	 * @param vector The vector part of this Quaternion.
	 * @param w The scalar part of this Quaternion.
	 */
	Quaternion(const Vector<3, float>& vector, const float w) : X(vector.X), Y(vector.Y), Z(vector.Z), W(w)
	{
	}

	/**
	 * @brief Gets this Quaternion with a magnitude of 1.
	 */
	Quaternion Normalized() const;

	/**
	 * @brief Gets the conjugsate of this Quaternion.
	 */
	Quaternion Conjugate() const;

	/**
	 * @brief Initializes a new Quaternion from an @p axis and @p angle.
	 * @param axis The rotation axis.
	 * @param angle The angle about the @p axis.
	 * @param radians A boolean indicating whether or not the specified @p angle is in radians.
	 *				  If true, @p angle is in radians; otherwise, @p angle is in degrees.
	 *				  This value defaults to true.
	 */
	static Quaternion AxisAngle(const Vector<3, float>& axis, float angle, bool radians = true);
};