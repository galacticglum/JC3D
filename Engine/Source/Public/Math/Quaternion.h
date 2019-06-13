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
	 * @brief Initializes a new Quaternion from a @p scalar value.
	 * @Note All components are set to the specified @p scalar value.
	 */
	explicit Quaternion(const float scalar) : Quaternion(scalar, scalar, scalar, scalar)
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
	 * @brief Get the vector part of this Quaternion.
	 */
	Vector<3, float> GetXYZ() const;

	/**
	 * @brief Set the vector part of this Quaternion.
	 */
	Quaternion& SetXYZ(const Vector<3, float>& vector);

	/**
	 * @brief Gets the magnitude of this Quaternion.
	 */
	float Magnitude() const;

	/**
	 * @brief Gets the square magnitude of this Quaternion.
	 */
	float SquareMagnitude() const;

	/**
	 * @brief Gets a copy of this Quaternion with negative elements.
	 * @returns A copy of this Quaternion whose elements have been negated.
	 */
	Quaternion Negative() const;

	/**
	 * @brief Gets this Quaternion with a magnitude of 1.
	 */
	Quaternion Normalized() const;

	/**
	 * @brief Gets the conjugate of this Quaternion.
	 */
	Quaternion Conjugate() const;

	/**
	 * @brief Gets the axis of this Quaternion.
	 */
	Vector<3, float> GetAxis() const;

	/**
	 * @brief Gets this Quaternion as euler angles.
	 */
	Vector<3, float> ToEulerAngles() const;

	/**
	 * @brief Assignment operator.
	 */
	Quaternion& operator=(const Quaternion& quaternion) = default;

	/**
	 * @brief Add two Quaternions together.
	 */
	Quaternion operator+(const Quaternion& quaternion) const;

	/**
	 * @brief Subtract two Quaternions.
	 */
	Quaternion operator-(const Quaternion& quaternion) const;

	/**
	 * @brief Multiply two Quaternions.
	 */
	Quaternion operator*(const Quaternion& quaternion) const;

	/**
	 * @brief Multiply a Quaternion by a scalar value.
	 */
	Quaternion operator*(float scalar) const;

	/**
	 * @brief Divide a Quaternion by a scalar value.
	 */
	Quaternion operator/(float scalar) const;

	/**
	 * @brief Index operator.
	 */
	float operator[](uint32_t index) const;

	/**
	 * @brief Add a Quaternion to this Quaternion.
	 */
	Quaternion& operator+=(const Quaternion& quaternion)
	{
		*this = *this + quaternion;
		return *this;
	}

	/**
	 * @brief Subtract a Quaternion from this Quaternion.
	 */
	Quaternion& operator-=(const Quaternion& quaternion)
	{
		*this = *this - quaternion;
		return *this;
	}

	/**
	 * @brief Multiply this Quaternion by a Quaternion.
	 */
	Quaternion& operator*=(const Quaternion& quaternion)
	{
		*this = *this * quaternion;
		return *this;
	}

	/**
	 * @brief Multiply this Quaternion by a scalar value.
	 */
	Quaternion& operator*=(const float scalar)
	{
		*this = *this * scalar;
		return *this;
	}

	/**
	 * @brief Divide this Quaternion by a scalar value.
	 */
	Quaternion& operator/=(const float scalar)
	{
		*this = *this / scalar;
		return *this;
	}

	/**
	 * @brief Negate operator.
	 */
	Quaternion operator-() const;

	/**
	 * @brief Determine whether a Quaternion is equal to this Quaternion.
	 */
	bool operator==(const Quaternion& quaternion) const;

	/**
	 * @brief Determine whether a Quaternion is not equal to this Quaternion.
	 */
	bool operator!=(const Quaternion& quaternion) const;

	/**
	 * @brief Rotate a quaternion by a vector.
	 */
	static Vector<3, float> Rotate(const Quaternion& quaternion, const Vector<3, float>& vector);

	/**
	 * @brief Create a rotation from the two specified direction vectors.
	 */
	static Quaternion Rotation(const Vector<3, float>& directionA, const Vector<3, float>& directionB);

	/**
	 * @brief Create a rotation from an @p axis and @p angle.
	 * @param axis The rotation axis.
	 * @param angle The angle about the @p axis.
	 * @param radians A boolean indicating whether or not the specified @p angle is in radians.
	 *				  If true, @p angle is in radians; otherwise, @p angle is in degrees.
	 *				  This value defaults to true.
	 */
	static Quaternion Rotation(const Vector<3, float>& axis, float angle, bool radians = true);

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
	 * @brief Create a Quaternion from Euler angles.
	 */
	static Quaternion FromEulerAngles(const Vector<3, float>& angles);

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
	 * @brief Stream operator.
	 */
	friend std::ostream& operator<<(std::ostream& stream, const Quaternion& right);

	/**
	 * @brief The identity Quaternion.
	 */
	static const Quaternion Identity;
};