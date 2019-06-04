/**
 * Author: AUTHOR
 * File Name: Vector3.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/04/2019
 * Modified Date: 06/04/2019
 * Description: Specialized implementation of a three-dimensional vector
 *				supporting arbitrary types.
 */

#pragma once

#include <Math/Vector.h>

 /**
  * @struct Vector Vector3.h "Public/Math/Vector3.h"
  * @brief A specialized implementation of a three-dimensional vector
  *		 supporting arbitrary types.
  * @tparam T The type of the vector elements.
  */
template<typename T>
struct Vector<3, T> : VectorBase<T, Vector<3, T>>
{
	union
	{
		/**
		 * @brief The Vector element data.
		 */
		std::array<T, Dimensions> Data;

		/**
		 * Components
		 */
		struct
		{
			T X;
			T Y;
			T Z;
		};
	};

	/**
	 * @brief A default empty constructor that initializes a new three-dimensional Vector: default initializes all vector elements.
	 */
	Vector() : Data()
	{
	}

	/**
	 * @brief Initializes a new three-dimensional Vector with a scalar value: all vector elements are initialized to the @p scalar value.
	 * @param scalar The value of all the elements in this Vector.
	 */
	explicit Vector(const T& scalar) : Vector()
	{
		Data = { scalar, scalar, scalar };
	}

	/**
	 * @brief Initializes a new three-dimensional Vector with a scalar value: all vector elements are initialized to the @p scalar value.
	 * @param x The x-component of this Vector.
	 * @param y The y-component of this Vector.
	 * @param z The z-component of this Vector.
	 */
	Vector(const T& x, const T& y, const T& z) : Vector()
	{
		Data = { x, y, z };
	}

	/**
	 * @brief Initializes a new three-dimensional Vector using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @param args The std::initializer_list<T> used to initialize this Vector's elements.
	 */
	Vector(const std::initializer_list<T> args)
	{
		assert(args.size() <= Dimensions);
		Data = args;
	}

	/**
	 * @brief Gets the cross product of the two vectors @p a and @p b.
	 * @returns A new vector that is perpendicular to vectors @P a and @p b.
	 */
	static Vector<3, T> Cross(const Vector<3, T>& a, const Vector<3, T>& b)
	{
		T rx = a.Y * b.Z - a.Z * b.Y;
		T ry = a.Z * b.X - a.X * b.Z;
		T rz = a.X * b.Y - a.Y * b.X;

		return Vector<3, T>(rx, ry, rz);
	}
};