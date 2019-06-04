/**
 * Author: Shon Verch
 * File Name: Vector.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/03/2019
 * Modified Date: 06/03/2019
 * Description: A generic n-dimensional vector implementation.
 */


#pragma once

#include <initializer_list>
#include <cassert>
#include <array>
#include <cmath>

// Macro definition for the generic vector template definition including typename restriction to only numeric types.
#define VECTOR_TEMPLATE template <std::size_t n, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>

/**
 * \class Vector Vector.h "Public/Math/Vector.h"
 * A generic vector implementation supporting arbitrary element types.
 * 
 * @tparam n The size of the vector (dimensionality).
 * @tparam T The type of the vector elements.
 */
VECTOR_TEMPLATE
struct Vector
{
	/**
	 * The \p Vector element data.
	 */
	std::array<T, n> Data;

	/**
	 * A default empty constructor that initializes a new \p Vector: default initializes all vector elements.
	 */
	Vector() {}

	/**
	 * Initializes a new \p Vector with a scalar value: all vector elements are initialized to the \p scalar value.
	 * 
	 * @param scalar The value of all the elements in this \p Vector.
	 */
	explicit Vector(const T& scalar)
	{
		for (auto& element : Data)
		{
			element = scalar;
		}
	}

	/**
	 * Initializes a new Vector using an \p std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * 
	 * @param args The \p std::initializer_list<T> used to initialize this Vector's elements.
	 */
	Vector(const std::initializer_list<T> args)
	{
		assert(args.size() <= n);
		Data = args;
	}

	/**
	 * Gets a copy of this \p Vector with negative elements.
	 */
	Vector<n, T> Negative() const
	{
		Vector<n, T> result;
		Foreach([](std::size_t index, const T& element)
		{
			result[index] = -element;
		});

		return result;
	}

	/**
	 * Gets the length of this \p Vector.
	 * @note The length of a vector is defined as the square root of the square of the vector:
	 *		 \f[|v| = \sqrt{\sum^n_{i=1} v_{i}^2},\f] where \f$v\f$ represents the vector and
	 *		 \f$n\f$ represents its dimension. The magnitude is implemented as \f$\sqrt{v\cdot v}\f$.
	 */
	T Magntiude() const
	{
		return static_cast<T>(std::sqrt(SquareMagnitude()));
	}

	/**
	 * Gets the squared length of this \p Vector.
	 */
	T SquareMagnitude() const
	{
		return Dot(this, this);
	}

	/**
	 * Makes this \p Vector have a magnitude of 1.
	 * @note This will change this Vector. If you want to keep the vector unchanged, use the \p Vector<n, T>.Normalized method instead.
	 */
	Vector<n, T>& Normalize()
	{
		T magnitude = Magntiude();
		if (magnitude == 0) return this;

		for (std::size_t i = 0; i < n; ++i)
		{
			Data[i] /= magnitude;
		}

		return this;
	}

	/**
	 * Gets this \p Vector with a magnitude of 1.
	 * @note This \p Vector remains unchanged. If you want to normalize the vector, use the \p Vector<n, T>.Normalize method instead.
	 */
	Vector<n, T> Normalized() const
	{
		return Vector<n, T>(this).Normalize();
	}

	/**
	 * Performs an operation on each element of this \p Vector.
	 * 
	 * @param operation The operation to apply on each element.
	 */
	void Foreach(void (*operation)(std::size_t index, const T& element)) const
	{
		for(std::size_t i = 0; i < n; ++i)
		{
			operation(i, Data[i]);
		}
	}

	/**
	 * Gets the value of the element in this Vector at the specified index.
	 * 
	 * @param index The index of the element whose value to retrieve.
	 */
	T& operator[] (const std::size_t index) { return GetAt(index); }

	/**
	 * Gets the value of the element in this Vector at the specified index.
	 *
	 * @param index The index of the element whose value to retrieve.
	 */
	const T& operator[] (const std::size_t index) const { return GetAt(index); }

	/**
	 * The number of dimensions that this Vector has.
	 */
	static std::size_t Dimensions() { return n; }

	/**
	 * Dot product of two vectors.
	 * @note The dot product is a value equal to the magnitudes of the two vectors multiplied
	 *	     together and then multiplied by the cosine of the angle between them.
	 */
	static T Dot(const Vector<n, T>& a, const Vector<n, T>& b)
	{
		T result;
		for (std::size_t i = 0; i < n; ++i)
		{
			result += a[i] * b[i];
		}

		return result;
	}

	/**
	 * Gets the distance between two vectors.
	 * @note The distance between two vectors is the same as the magnitude of their difference.
	 */
	static T Distance(const Vector<n, T>& a, const Vector<n, T>&  b)
	{
		return (a - b).Magnitude();
	}

	/**
	 * Gets the square distance between two vectors.
	 * @note The square distance between two vectors is the same as the square magnitude of their difference.
	 */
	static T SquareDistance(const Vector<n, T>& a, const Vector<n, T>&  b)
	{
		return (a - b).SquareMagnitude();
	}

	/**
	 * Linearly interpolates between two vectors.
	 * 
	 * Interpolates between the vectors \p a and \p y by the interpolant \y t.
	 * The parameter \y t is clamped within the range [0, 1].
	 */
	static Vector<n, T> Lerp(const Vector<n, T>& a, const Vector<n, T>& b, T t)
	{
		return a + (b - a) * t;
	}
private:
	/**
	 * Gets the value of the element in this Vector at the specified index.
	 *
	 * @param index The index of the element whose value to retrieve.
	 */
	const T& GetAt(const std::size_t index)
	{
		assert(index >= 0 && index < n);
		return Data.at(index);
	}
};

/**
 * Negates all the elements of the vector.
 */
VECTOR_TEMPLATE
Vector<n, T> operator -(const Vector<n, T>& vector)
{
	return vector.Negative();
}

/**
 * Adds two n-dimensional vectors of type T by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator +(const Vector<n, T>& left, const Vector<n, T>& right)
{
	Vector<n, T> result;
	for (std::size_t i = 0; i < n; ++i)
	{
		result[i] = left[i] + right[i];
	}

	return result;
}

/**
 * Subtracts two n-dimensional vectors of type T by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator -(const Vector<n, T>& left, const Vector<n, T>& right)
{
	return left + -right;
}

/**
 * Multiplies two n-dimensional vectors of type T by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator *(const Vector<n, T>& left, const Vector<n, T>& right)
{
	Vector<n, T> result;
	for (std::size_t i = 0; i < n; ++i)
	{
		result[i] = left[i] * right[i];
	}

	return result;
}

/**
 * Divides two n-dimensional vectors of type T by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator /(const Vector<n, T>& left, const Vector<n, T>& right)
{
	Vector<n, T> result;
	for (std::size_t i = 0; i < n; ++i)
	{
		result[i] = left[i] / right[i];
	}

	return result;
}

/**
 * Multiplies an n-dimensional vectors of type T by a scalar value, by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator *(const Vector<n, T>& left, T scalar)
{
	Vector<n, T> result;
	for (std::size_t i = 0; i < n; ++i)
	{
		result[i] = left[i] * scalar;
	}

	return result;
}

/**
 * Multiplies an n-dimensional vectors of type T by a scalar value, by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator *(T scalar, const Vector<n, T>& right)
{
	return right * scalar;
}

/**
 * Divides an n-dimensional vectors of type T by a scalar value, by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator /(const Vector<n, T>& left, T scalar)
{
	Vector<n, T> result;
	for (std::size_t i = 0; i < n; ++i)
	{
		result[i] = left[i] / scalar;
	}

	return result;
}

/**
 * Divides a scalar value by an n-dimensional vectors of type T, by component.
 * @note In reality, we are multiplying the reciprocal of the vector by a scalar.
 */
VECTOR_TEMPLATE
Vector<n, T> operator /(T scalar, const Vector<n, T>& right)
{
	Vector<n, T> result;
	for (std::size_t i = 0; i < n; ++i)
	{
		result[i] = scalar / right[i];
	}

	return result;
}

/**
 * Adds an n-dimensional vector of type T to another same-sized vector of type T.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator +=(Vector<n, T>& left, const Vector<n, T>& right)
{
	right.Foreach([](std::size_t index, const T& element)
	{
		left[index] += element;
	});

	return left;
}

/**
 * Subtracts an n-dimensional vector of type T from another same-sized vector of type T.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator -=(Vector<n, T>& left, const Vector<n, T>& right)
{
	left += -right;
	return left;
}

/**
 * Multiplies an n-dimensional vector of type T by another same-sized vector of type T.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator *=(Vector<n, T>& left, const Vector<n, T>& right)
{
	right.Foreach([](std::size_t index, const T& element)
	{
		left[index] *= element;
	});
	
	return left;
}

/**
 * Divides an n-dimensional vector of type T by another same-sized vector of type T.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator /=(Vector<n, T>& left, const Vector<n, T>& right)
{
	right.Foreach([](std::size_t index, const T& element)
	{
		left[index] /= element;
	});

	return left;
}

/**
 * Multiplies an n-dimensional vector of type T by a scalar value of type T.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator *=(Vector<n, T>& left, T scalar)
{
	for(std::size_t i = 0; i < n; ++i)
	{
		left[i] *= scalar;
	}
	
	return left;
}

/**
 * Divides an n-dimensional vector of type T by a scalar value of type T.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator /=(Vector<n, T>& left, T scalar)
{
	for (std::size_t i = 0; i < n; ++i)
	{
		left[i] /= scalar;
	}
	
	return left;
}

/**
 * Determines whether two same-sized vectors of the same type are equal.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator ==(const Vector<n, T>& left, const Vector<n, T>& right)
{
	for (std::size_t i = 0; i < n; ++i)
	{
		if (left[i] != right[i]) return false;
	}

	return true;
}

/**
 * Determines whether two same-sized vectors of the same type are not equal.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator !=(const Vector<n, T>& left, const Vector<n, T>& right)
{
	return !(left == right);
}