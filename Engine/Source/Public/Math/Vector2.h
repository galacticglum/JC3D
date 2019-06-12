/**
 * Author: Shon Verch
 * File Name: Vector2.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/04/19
 * Modified Date: 06/07/19
 * Description: Specialized implementation of a two-dimensional vector 
 *				supporting arbitrary types.
 */

#pragma once

#include <Math/Vector.h>

/**
 * @struct Vector Vector2.h
 * @brief A specialized implementation of a two-dimensional vector
 *		  supporting arbitrary types. 
 * @tparam T The type of the vector elements.
 */
template<typename T>
struct Vector<2, T> : VectorBase<2, T, Vector<2, T>>
{
	union
	{
		/**
		 * @brief The Vector element data.
		 */
		std::array<T, 2> Data;

		/**
		 * Components
		 */
		struct
		{
			T X;
			T Y;
		};
	};

	/**
	 * @brief A default empty constructor that initializes a new two-dimensional Vector: default initializes all vector elements.
	 */
	Vector() : Data()
	{
	}

	/**
	 * @brief Initializes a new two-dimensional Vector with a scalar value: all vector elements are initialized to the @p scalar value.
	 * @param scalar The value of all the elements in this Vector.
	 */
	explicit Vector(const T& scalar) : Vector()
	{
		Data = { scalar, scalar };
	}

	/**
	 * @brief Initializes a new two-dimensional Vector given the components.
	 * @param x The x-component of this Vector.
	 * @param y The y-component of this Vector.
	 */
	Vector(const T& x, const T& y) : Vector()
	{
		Data = { x, y };
	}

	/**
	 * @brief Initializes a new two-dimensional Vector using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @param args The std::initializer_list<T> used to initialize this Vector's elements.
	 */
	Vector(const std::initializer_list<T> args)
	{
		assert(args.size() <= Size());
		Data = args;
	}

	/**
	 * @brief Gets the vector perpendicular to the specified @p vector.
	 * @returns A new Vector that is perpendicular to the specified @p vector.
	 */
	static Vector<2, T> Perpendicular(const Vector<2, T>& vector)
	{
		return Vector<2, T>(-vector.Y, vector.X);
	}
};