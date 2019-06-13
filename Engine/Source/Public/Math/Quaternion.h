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
	 * @brief Initializes a new Quaternion given the components.
	 * @param x The x-component of this Quaternion. Defaults to zero.
	 * @param y The y-component of this Quaternion. Defaults to zero.
	 * @param z The z-component of this Quaternion. Defaults to zero.
	 * @param w The w-component of this Quaternion. Defaults to one.
	 */
	explicit Quaternion(const float x = 0, const float y = 0, const float z = 0, const float w = 1)
		: X(x), Y(y), Z(z), W(w)
	{
	}

	/**
	 * @brief Initialize a new Quaternion from a scalar.
	 */
	explicit Quaternion(const float scalar)
		: X(scalar), Y(scalar), Z(scalar), W(scalar)
	{
	}

	/**
	 * @brief Initialize a new Quaternion from a Vector4f.
	 */
	explicit Quaternion(const Vector<4, float>& vector)
		: X(vector.X), Y(vector.Y), Z(vector.Z), W(vector.W)
	{
	}

	/**
	 * @brief Initialize a new Quaternion from a @p vector 
	 *		  and scalar, @p w, part.
	 */
	explicit Quaternion(const Vector<3, float>& vector, const float w)
		: X(vector.X), Y(vector.Y), Z(vector.Z), W(w)
	{
		
	}

	/**
	 * @brief Gets the vector part of this Quaternion.
	 */
	Vector<3, float> GetVectorPart() const
	{
		return Vector<3, float>(X, Y, Z);
	}

	/**
	 * @brief Sets the vector part of this Quaternion.
	 */
	Quaternion& SetVectorPart(const Vector<3, float>& vector)
	{
		X = vector.X;
		Y = vector.Y;
		Z = vector.Z;

		return *this;
	}

	/**
	 * @brief Get the semi-rotational axis of this Quaternion.
	 */
	Vector<3, float> GetAxis() const;

	/**
	 * @brief The raw pitch in radians.
	 */
	float GetPitch() const;

	/**
	 * @brief The raw yaw in radians.
	 */
	float GetYaw() const;

	/**
	 * @brief The raw roll in radians.
	 */
	float GetRoll() const;

	/**
	 * @brief Gets this Quaternion in euler angles.
	 */
	Vector<3, float> ToEulerAngles() const;

	/**
	 * @brief Gets a new Quaternion representing the sum of this Quaternion and @p quaternion.
	 */
	Quaternion operator+(const Quaternion& quaternion) const;

	/**
	 * @brief Gets a new Quaternion representing the difference of this Quaternion and @p quaternion.
	 */
	Quaternion operator-(const Quaternion& quaternion) const;

	/**
	 * @brief Gets a new Quaternion representing the product of this Quaternion and @p quaternion.
	 */
	Quaternion operator*(const Quaternion& quaternion) const;

	/**
	 * @brief Gets a new Quaternion representing the product of this Quaternion and @p scalar.
	 */
	Quaternion operator*(float scalar) const;

	/**
	 * @brief Gets a new Quaternion representing the quotient of this Quaternion and @p scalar.
	 */
	Quaternion operator/(float scalar) const;

	/**
	 * @brief Adds the @p quaternion to this Quaternion.
	 */
	Quaternion& operator+=(const Quaternion& quaternion)
	{
		*this = *this + quaternion;
		return *this;
	}

	/**
	 * @brief Subtracts @p quaternion from this Quaternion.
	 */
	Quaternion& operator-=(const Quaternion& quaternion)
	{
		*this = *this - quaternion;
		return *this;
	}

	/**
	 * @brief Multiplies this Quaternion by @p quaternion.
	 */
	Quaternion& operator*=(const Quaternion& quaternion)
	{
		*this = *this * quaternion;
		return *this;
	}

	/*
	 * @brief Multiplies this Quaternion by @p scalar.
	 */
	Quaternion& operator*=(const float scalar)
	{
		*this = *this * scalar;
		return *this;
	}

	/**
	 * @brief Negate this Quaternion.
	 */
	Quaternion operator-() const;

	/**
	 * @brief Determine whether this Quaternion equals @p quaternion.
	 */
	bool operator==(const Quaternion& quaternion) const;

	/**
	 * @brief Determine whether this Quaternion does not equals @p quaternion.
	 */
	bool operator!=(const Quaternion& quaternion) const;

	/**
	 * @brief The identity quaternion.
	 */
	static const Quaternion Identity;

	/**
	 * @brief Creates a new Quaternion from euler angles given by the three-dimensional Vector @p angles.
	 * @note @p angles is a three-dimensional vector (x, y, z) that corresponds to (pitch, yaw, roll).
	 */
	static Quaternion FromEulerAngles(const Vector<3, float>& angles);

	/**
	 * @brief Rotates @p vector by @p quaternion.
	 */
	static Vector<3, float> Rotate(const Quaternion& quaternion, const Vector<3, float>& vector);

	/**
	 * @brief Creates a Quaternion from two unit-vector directions.
	 */
	static Quaternion Rotation(const Vector<3, float>& a, const Vector<3, float>& b);

	/**
	 * @brief Creates a Quaternion from an angle and axis.
	 */
	static Quaternion Rotation(float radians, const Vector<3, float>& axis);

	/**
	 * @brief Creates a Quaternion rotated about the x-axis by @p radians.
	 */
	static Quaternion RotationX(const float radians)
	{
		const float angle = radians * 0.5f;
		return Quaternion(sin(angle), 0.0f, 0.0f, cos(angle));
	}

	/**
	 * @brief Creates a Quaternion rotated about the y-axis by @p radians.
	 */
	static Quaternion RotationY(const float radians)
	{
		const float angle = radians * 0.5f;
		return Quaternion(0.0f, sin(angle), 0.0f, cos(angle));
	}

	/**
	 * @brief Creates a Quaternion rotated about the z-axis by @p radians.
	 */
	static Quaternion RotationZ(const float radians)
	{
		const float angle = radians * 0.5f;
		return Quaternion(0.0f, 0.0f, sin(angle), cos(angle));
	}

	/**
	 * @brief Gets the dot product of this Quaternion and @p other.
	 */
	float Dot(const Quaternion& other) const;

	/**
	 * @brief Gets the conjugate of this Quaternion.
	 */
	Quaternion Conjugate() const;
};