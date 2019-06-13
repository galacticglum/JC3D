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
#include <Math/MathFunctions.h>
#include <Math/Matrix.h>

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
	 * @brief Initializes a new Quaternion with its components set to zero.
	 */
	Quaternion() : X(0), Y(0), Z(0), W(0)
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
	 * @brief Create a rotation from an @p axis and @p angle.
	 * @param axis The rotation axis.
	 * @param angle The angle about the @p axis.
	 * @param isRadians A boolean indicating whether or not the specified @p angle is in radians.
	 *				  If true, @p angle is in radians; otherwise, @p angle is in degrees.
	 *				  This value defaults to true.
	 */
	Quaternion(const Vector<3, float>& axis, float angle, const bool isRadians = true)
	{
		if(!isRadians)
		{
			angle = MathFunctions::DegreesToRadians(angle);
		}

		const float halfAngle = 0.5f * angle;
		W = std::cosf(halfAngle);
		X = axis.X * std::sinf(halfAngle);
		Y = axis.Y * std::sinf(halfAngle);
		Z = axis.Z * std::sinf(halfAngle);
	}

	/**
	 * @brief Initialize a Quaternion from an axis.
	 */
	explicit Quaternion(const Vector<3, float>& axis)
	{
		W = 0.0f;
		X = axis.X;
		Y = axis.Y;
		Z = axis.Z;
	}

	/**
	 * @brief Gets axis-angle representation of this Quaternion as a Vector4f.
	 */
	Vector<4, float> ToAxisAngle() const;

	/**
	 * @brief Gets the rotation matrix from this Quaternion.
	 */
	Matrix<4, 4, float> ToMatrix() const;

	/**
	 * @brief Negate operator.
	 */
	Quaternion operator-() const;

	/**
	 * @brief Gets the magnitude of this Quaternion.
	 */
	float Magnitude() const;

	/**
	 * @brief Gets the square magnitude of this Quaternion.
	 */
	float SquareMagnitude() const;

	/**
	 * @brief Gets this Quaternion with a magnitude of 1.
	 */
	Quaternion Normalized() const;

	/**
	 * @brief Gets the conjugate of this Quaternion.
	 */
	Quaternion Conjugate() const;

	/**
	 * @brief Gets this Quaternion as euler angles.
	 */
	Vector<3, float> ToEulerAngles() const;

	/**
	 * @brief Gets the vector part of this Quaternion.
	 */
	Vector3f GetAxis() const
	{
		return Vector3f(X, Y, Z);
	}

	/**
	 * @brief Makes the specified @p quaternion have a magnitude of 1.
	 * @note This will change this Quaternion. If you want to keep the quaternion unchanged,
	 *		 use the Quaternion::Normalized method instead.
	 * @returns A reference to @p quaternion.
	 */
	static Quaternion& Normalize(Quaternion& quaternion);

	/**
	 * @brief Dot product of two quaternions.
	 * @returns The dot product of quaternion @p a and @p b.
	 */
	static float Dot(const Quaternion& a, const Quaternion& b);

	/**
	 * @brief Linear interpolation between two Quaternions by the interpolant.
	 */
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t, bool shortest);

	/**
	 * @brief Spherical interpolation between two Quaternions by the interpolant.
	 */
	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t, bool shortest);

	/**
	 * @brief Convert this Quaternion to its string representation.
	 */
	std::string ToString() const;

	/**
	 * @brief Create a Quaternion from Euler angles.
	 * @note Angles represents (roll, pitch, yaw) which
	 *		 maps to (x, y, z) components of the @p angles vector.
	 */
	static Quaternion FromEulerAngles(const Vector<3, float>& angles);

	/**
	 * @brief Rotate a quaternion by a vector.
	 */
	static Vector<3, float> Rotate(const Quaternion& quaternion, const Vector<3, float>& vector);

	/**
	 * @brief Create a rotation from the two specified direction vectors.
	 */
	static Quaternion Rotation(const Vector<3, float>& u, const Vector<3, float>& v);

	/**
	 * @brief Create a rotation from an @p axis and @p angle.
	 * @param axis The rotation axis.
	 * @param angle The angle about the @p axis.
	 * @param radians A boolean indicating whether or not the specified @p angle is in radians.
	 *				  If true, @p angle is in radians; otherwise, @p angle is in degrees.
	 *				  This value defaults to true.
	 */
	static Quaternion Rotation(const Vector<3, float>& axis, const float angle, const bool radians = true)
	{
		return Quaternion(axis, angle, radians);
	}

	/**
	 * @brief Stream operator.
	 */
	friend std::ostream& operator<<(std::ostream& stream, const Quaternion& right);

	/**
	 * @brief The identity Quaternion.
	 */
	static const Quaternion Identity;
};