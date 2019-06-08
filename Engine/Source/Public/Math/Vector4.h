/**
 * Author: AUTHOR
 * File Name: Vector4.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/04/2019
 * Modified Date: 06/04/2019
 * Description: Specialized implementation of a four-dimensional vector
 *				supporting arbitrary types.
 */

#pragma once

#include <Math/Vector.h>

 /**
  * @struct Vector Vector4.h
  * @brief A specialized implementation of a four-dimensional vector
  *		   supporting arbitrary types.
  * @tparam T The type of the vector elements.
  */
template<typename T>
struct Vector<4, T> : VectorBase<T, Vector<4, T>>
{
	union
	{
		/**
		 * @brief The Vector element data.
		 */
		std::array<T, 4> Data;

		/**
		 * Components
		 */
		struct
		{
			T X;
			T Y;
			T Z;
			T W;
		};
	};

	/**
	 * @brief A default empty constructor that initializes a new four-dimensional Vector: default initializes all vector elements.
	 */
	Vector() : Data()
	{
	}

	/**
	 * @brief Initializes a new four-dimensional Vector with a scalar value: all vector elements are initialized to the @p scalar value.
	 * @param scalar The value of all the elements in this Vector.
	 */
	explicit Vector(const T& scalar) : Vector()
	{
		Data = { scalar, scalar, scalar, scalar };
	}

	/**
	 * @bref Initializes a new four-dimensional vector from a three-dimensional vector representing the x, y, and z components
	 *		 of this Vector and a scalar value, @p w, representing the w-component of this Vector.
	 * @param vector A three-dimensional vector whose x, y, and z components are used for this Vector.
	 * @param w The z-component of this Vector.
	 */
	Vector(const Vector<3, T>& vector, const T& w) : Vector()
	{
		Data = { vector.X, vector.Y, vector.Z, w };
	}

	/**
	 * @brief Initializes a new four-dimensional Vector with a scalar value: all vector elements are initialized to the @p scalar value.
	 * @param x The x-component of this Vector.
	 * @param y The y-component of this Vector.
	 * @param z The z-component of this Vector.
	 * @param w The w-component of this Vector.
	 */
	Vector(const T& x, const T& y, const T& z, const T& w) : Vector()
	{
		Data = { x, y, z, w };
	}

	/**
	 * @brief Initializes a new four-dimensional Vector using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @param args The std::initializer_list<T> used to initialize this Vector's elements.
	 */
	Vector(const std::initializer_list<T> args)
	{
		assert(args.size() <= Size());
		Data = args;
	}
};