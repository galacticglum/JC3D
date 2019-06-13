/**
 * Author: Shon Verch
 * File Name: Vector3.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/04/2019
 * Modified Date: 06/07/2019
 * Description: Specialized implementation of a three-dimensional vector
 *				supporting arbitrary types.
 */

#pragma once

 /**
  * @struct Vector Vector3.h
  * @brief A specialized implementation of a three-dimensional vector
  *		   supporting arbitrary types.
  * @tparam T The type of the vector elements.
  */
template<typename T>
struct Vector<3, T> : VectorBase<3, T, Vector<3, T>>
{
	union
	{
		/**
		 * @brief The Vector element data.
		 */
		std::array<T, 3> Data;

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
	 * @bref Initializes a new three-dimensional vector from a two-dimensional vector representing the x and y components
	 *		 of this Vector and a scalar value, @p z, representing the z-component of this Vector.
	 * @param vector A two-dimensional vector whose x and y components are used for this Vector.
	 * @param z The z-component of this Vector.
	 */
	Vector(const Vector<2, T>& vector, const T& z) : Vector()
	{
		Data = { vector.X, vector.Y, z };
	}

	/**
	 * @brief Initializes a new three-dimensional Vector given the components.
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
		assert(args.size() <= Size());
		std::size_t index = 0;
		for (auto it = args.begin(); it != args.end(); ++it)
		{
			Data.at(index++) = *it;
		}
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

	/**
	 * @brief All components are zero.
	 */
	static const Vector<3, T> Zero;

	/**
	 * @brief All components are one.
	 */
	static const Vector<3, T> One;

	/**
	 * @brief Unit up vector.
	 */
	static const Vector<3, T> Up;

	/**
	 * @brief Unit down vector.
	 */
	static const Vector<3, T> Down;

	/**
	 * @brief Unit left vector.
	 */
	static const Vector<3, T> Left;

	/**
	 * @brief Unit right vector.
	 */
	static const Vector<3, T> Right;

	/**
	 * @brief Unit forward vector.
	 */
	static const Vector<3, T> Forward;

	/**
	 * @brief Unit back vector.
	 */
	static const Vector<3, T> Back;

	/**
	 * @brief X-axis vector.
	 */
	static const Vector<3, T> XAxis;

	/**
	 * @brief Y-axis vector.
	 */
	static const Vector<3, T> YAxis;

	/**
	 * @brief Z-axis vector.
	 */
	static const Vector<3, T> ZAxis;
};

template <typename T>
const Vector<3, T> Vector<3, T>::Zero = Vector<3, T>(0);

template <typename T>
const Vector<3, T> Vector<3, T>::One = Vector<3, T>(1);

template <typename T>
const Vector<3, T> Vector<3, T>::Up = Vector<3, T>(0, 1, 0);

template <typename T>
const Vector<3, T> Vector<3, T>::Down = Vector<3, T>(0, -1, 0);

template <typename T>
const Vector<3, T> Vector<3, T>::Left = Vector<3, T>(-1, 0, 0);

template <typename T>
const Vector<3, T> Vector<3, T>::Right = Vector<3, T>(1, 0, 0);

template <typename T>
const Vector<3, T> Vector<3, T>::Forward = Vector<3, T>(0, 0, 1);

template <typename T>
const Vector<3, T> Vector<3, T>::Back = Vector<3, T>(0, 0, -1);

template <typename T>
const Vector<3, T> Vector<3, T>::XAxis = Vector<3, T>(1, 0, 0);

template <typename T>
const Vector<3, T> Vector<3, T>::YAxis = Vector<3, T>(0, 1, 0);

template <typename T>
const Vector<3, T> Vector<3, T>::ZAxis = Vector<3, T>(0, 0, 1);