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

#define VECTOR_TEMPLATE template <std::size_t n, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>

/**
 * A generic vector implementation supporting arbitrary element types.
 * 
 * @tparam n The size of the vector (dimensionality).
 * @tparam T The type of the vector elements.
 */
VECTOR_TEMPLATE
struct Vector
{
	/**
	 * The Vector element data.
	 */
	std::array<T, n> Data;

	/**
	 * A default empty constructor that initializes a new Vector: default initializes all vector elements.
	 */
	Vector() {}

	/**
	 * Initializes a new Vector with a scalar value: all vector elements are initialized to the scalar value.
	 * 
	 * @param scalar The value of all the elements in this Vector.
	 */
	explicit Vector(const T& scalar)
	{
		for (auto& element : Data)
		{
			element = scalar;
		}
	}

	/**
	 * Initializes a new Vector using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * 
	 * @param args The std::initializer_list<T> used to initialize this Vector's elements.
	 */
	Vector(const std::initializer_list<T> args)
	{
		assert(args.size() <= n);
		Data = args;
	}

	/**
	 * The number of dimensions that this Vector has.
	 */
	static std::size_t Dimensions() { return n; }

	/*
	 * Gets a copy of this Vector with negative elements.
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
	 * Performs an operation on each element of this Vector.
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