/**
 * Author: Shon Verch
 * File Name: Vector.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/03/2019
 * Modified Date: 06/04/2019
 * Description: A generic n-dimensional vector implementation.
 */

#pragma once

#include <initializer_list>
#include <cassert>
#include <array>
#include <cmath>
#include <type_traits>

/**
 * @struct VectorBase Vector.h "Public/Math/Vector.h"
 * @brief A base generic vector supporting arbitrary element types.
 * @note Serves a wrapper around derived vector classes.
 * 
 * @tparam T the type of the vector elements.
 * @tparam Derived The type of the derived vector class.
 *		   @p Derived is assumed to be a Vector type (base or subclass of Vector).
 */
template<typename T, typename Derived>
struct VectorBase
{
	/**
	 * @brief Get the derived instance.
	 */
	Derived& GetDerived() { return static_cast<Derived&>(*this); }

	/**
	 * @brief Get the derived instance.
	 */
	const Derived& GetDerived() const { return static_cast<const Derived&>(*this); }

	/**
	 * @brief The size of this Vector.
	 */
	const std::size_t Size = std::extent<decltype(GetDerived().Data)>::value;

	/**
	 * @brief The number of dimensions of this Vector.
	 * @note This is the same as VectorBase::Size.
	 */
	const std::size_t Dimensions = Size;

	/**
	 * @brief Zero out this Vector.
	 * @note Reinitialize all elements to their default value.
	 */
	void Zero()
	{
		std::fill(std::begin(GetDerived().Data), std::end(GetDerived().Data), T());
	}

	using Iterator = T*;
	using ConstIterator = const T*;

	/**
	 * @brief An iterator pointing to the beginning of this Vector.
	 */
	Iterator Begin() { return &GetDerived().Data[0]; }

	/**
	 * @brief An iterator pointing to the end of this Vector.
	 */
	Iterator End() { return &GetDerived().Data[0] + Size; }

	/**
	 * @brief An iterator pointing to the beginning of this Vector.
	 */
	ConstIterator Begin() const { return Begin(); }

	/**
	 * @brief An iterator pointing to the end of this Vector.
	 */
	ConstIterator End() const { return End(); }

	/**
	 * @brief Gets the length of this Vector.
	 * @note The length of a vector is defined as the square root of the square of the vector:
	 *		 @f[|v| = \sqrt{\sum^n_{i=1} v_{i}^2},@f] where @f$v@f$ represents the vector and
	 *		 @f$n@f$ represents its dimension. The magnitude is implemented as @f$\sqrt{v\cdot v}@f$.
	 * @returns The length of this Vector.
	 */
	T Magnitude() const
	{
		return static_cast<T>(std::sqrt(SquareMagnitude()));
	}

	/**
	 * @brief Gets the squared length of this Vector.
	 * @returns The squared length of this Vector.
	 */
	T SquareMagnitude() const
	{
		const Derived& instance = GetDerived();
		return Dot(instance, instance);
	}

	/**
	 * @brief Makes the specified @p vector have a magnitude of 1.
	 * @note This will change this Vector. If you want to keep the vector unchanged,
	 *		 use the Vector<n, T>::Normalized method instead.
	 * @returns A reference to @p vector.
	 */
	static Derived& Normalize(const Derived& vector)
	{
		T magnitude = vector.Magnitude();
		if (magnitude == 0) return vector;

		for (std::size_t i = 0; i < vector.Size; ++i)
		{
			vector[i] /= magnitude;
		}

		return vector;
	}

	/**
	 * @brief Gets this Vector with a magnitude of 1.
	 * @note This Vector remains unchanged. If you want to normalize the vector,
	 *		 use the Vector<n, T>::Normalize method instead.
	 *	@returns A normalized copy of this Vector.
	 */
	Derived Normalized() const
	{
		return Derived(GetDerived()).Normalize();
	}

	/**
	 * @brief Performs an @p operation on each element of this Vector.
	 * @param operation The operation to apply on each element.
	 */
	void Foreach(void(*operation)(std::size_t index, const T& element)) const
	{
		for (std::size_t i = 0; i < Size; ++i)
		{
			operation(i, this[i]);
		}
	}

	/**
	 * @brief Gets a copy of this Vector with negative elements.
	 * @returns A copy of this Vector whose elements have been negated.
	 */
	Derived Negative() const
	{
		Derived result;
		GetDerived().Foreach([](std::size_t index, const T& element)
		{
			result[index] = -element;
		});

		return result;
	}

	/**
	 * @brief Dot product of two vectors.
	 * @note The dot product is a value equal to the magnitudes of the two vectors multiplied
	 *	     together and then multiplied by the cosine of the angle between them.
	 * @returns The dot product of vector @p a and @p b.
	 */
	static T Dot(const Derived& a, const Derived& b)
	{
		T result;

		const std::size_t size = a.Size;
		for (std::size_t i = 0; i < size; ++i)
		{
			result += a[i] * b[i];
		}

		return result;
	}

	/**
	 * @brief Gets the distance between two vectors.
	 * @note The distance between two vectors is the same as the magnitude of their difference.
	 * @returns The distance between vector @p a and @p b.
	 */
	static T Distance(const Derived& a, const Derived&  b)
	{
		return (a - b).Magnitude();
	}

	/**
	 * @brief Gets the square distance between two vectors.
	 * @note The square distance between two vectors is the same as the square magnitude of their difference.
	 * @returns The square distance between vector @p and @p b.
	 */
	static T SquareDistance(const Derived& a, const Derived&  b)
	{
		return (a - b).SquareMagnitude();
	}

	/**
	 * @brief Linearly interpolates between two vectors.
	 * @note Interpolates between the vectors @p a and @p b by the interpolant @p t.
	 *		 The parameter @p t is clamped within the range [0, 1].
	 *	@returns A new interpolated vector between @p a and @p b.
	 */
	static Derived Lerp(const Derived& a, const Derived& b, T t)
	{
		return a + (b - a) * t;
	}

	/**
	 * @brief Gets the value of the element in this Vector at the specified @p index.
	 * @param index The index of the element whose value to retrieve.
	 * @returns A reference to the value of the element at the specified @p index.
	 */
	const T& GetAt(const std::size_t index)
	{
		assert(index >= 0 && index < Size());
		return GetDerived().Data[index];
	}

	/**
	  * @brief Gets the value of the element in this Vector at the specified @p index.
	  * @param index The index of the element whose value to retrieve.
	  * @returns A reference to the value of the element at the specified @p index.
	  */
	T& operator[] (const std::size_t index) { return GetAt(index); }

	/**
	 * @brief Gets the value of the element in this Vector at the specified @p index
	 * @param index The index of the element whose value to retrieve.
	 * @returns A reference to the value of the element at the specified @p index.
	 */
	const T& operator[] (const std::size_t index) const { return GetAt(index); }
};

/**
 * @brief Macro definition for the generic vector template typename definition.
 * @note Restricts to arithmetic types only.
 */
#define VECTOR_TYPENAME_TEMPLATE typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type

/**
 * @brief Macro definition for the generic vector template definition including typename restriction to only numeric types.
 */
#define VECTOR_TEMPLATE template <std::size_t n, VECTOR_TYPENAME_TEMPLATE>

/**
 * @struct Vector Vector.h "Public/Math/Vector.h"
 * @brief A generic vector implementation supporting arbitrary element types.
 * 
 * @tparam n The size of the vector (dimensionality).
 * @tparam T The type of the vector elements.
 */
VECTOR_TEMPLATE
struct Vector : VectorBase<T, Vector<n, T>>
{
	union
	{
		/**
		 * @brief The Vector element data.
		 */
		std::array<T, n> Data;
	};

	/**
	 * @brief A default empty constructor that initializes a new Vector: default initializes all vector elements.
	 */
	Vector() : Data()
	{
	}

	/**
	 * @brief Initializes a new Vector with a scalar value: all vector elements are initialized to the @p scalar value.
	 * @param scalar The value of all the elements in this Vector.
	 */
	explicit Vector(const T& scalar) : Vector()
	{
		for (auto& element : Data)
		{
			element = scalar;
		}
	}

	/**
	 * @brief Initializes a new Vector using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @param args The std::initializer_list<T> used to initialize this Vector's elements.
	 */
	Vector(const std::initializer_list<T> args)
	{
		assert(args.size() <= n);
		Data = args;
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
 * @brief Adds two n-dimensional vectors of type T by component.
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
 * @brief Subtracts two n-dimensional vectors of type T by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator -(const Vector<n, T>& left, const Vector<n, T>& right)
{
	return left + -right;
}

/**
 * @brief Multiplies two n-dimensional vectors of type T by component.
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
 * @brief Divides two n-dimensional vectors of type T by component.
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
 * @brief Multiplies an n-dimensional vectors of type T by a scalar value, by component.
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
 * @brief Multiplies an n-dimensional vectors of type T by a scalar value, by component.
 */
VECTOR_TEMPLATE
Vector<n, T> operator *(T scalar, const Vector<n, T>& right)
{
	return right * scalar;
}

/**
 * @brief Divides an n-dimensional vectors of type T by a scalar value, by component.
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
 * @brief Divides a scalar value by an n-dimensional vectors of type T, by component.
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
 * @brief Adds an n-dimensional vector of type T to another same-sized vector of type T.
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
 * @brief Subtracts an n-dimensional vector of type T from another same-sized vector of type T.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator -=(Vector<n, T>& left, const Vector<n, T>& right)
{
	left += -right;
	return left;
}

/**
 * @brief Multiplies an n-dimensional vector of type T by another same-sized vector of type T.
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
 * @brief Divides an n-dimensional vector of type T by another same-sized vector of type T.
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
 * @brief Multiplies an n-dimensional vector of type T by a scalar value of type T.
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
 * @brief Divides an n-dimensional vector of type T by a scalar value of type T.
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
 * @brief Determines whether two same-sized vectors of the same type are equal.
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
 * @brief Determines whether two same-sized vectors of the same type are not equal.
 */
VECTOR_TEMPLATE
Vector<n, T>& operator !=(const Vector<n, T>& left, const Vector<n, T>& right)
{
	return !(left == right);
}

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

/**
 * @brief Two-dimensional vector with arbitrary element type.
 */
template<VECTOR_TYPENAME_TEMPLATE>
using Vector2 = Vector<2, T>;

/**
 * @brief Three-dimensional vector with arbitrary element type.
 */
template<VECTOR_TYPENAME_TEMPLATE>
using Vector3 = Vector<3, T>;

/**
 * @brief Four-dimensional vector with arbitrary element type.
 */
template<VECTOR_TYPENAME_TEMPLATE>
using Vector4 = Vector<4, T>;

/**
 * @brief Two-dimensional floating-point precision vector.
 */
using Vector2f = Vector2<float>;

/**
 * @brief Three-dimensional floating-point precision vector.
 */
using Vector3f = Vector3<float>;

/**
 * @brief Four-dimensional floating-point precision vector.
 */
using Vector4f = Vector4<float>;

/**
 * @brief Two-dimensional double-precision vector.
 */
using Vector2d = Vector2<double>;

/**
 * @brief Three-dimensional double-precision vector.
 */
using Vector3d = Vector3<double>;

/**
 * @brief Four-dimensional double-precision vector.
 */
using Vector4d = Vector4<double>;

/**
 * @brief Two-dimensional integer vector.
 */
using Vector2i = Vector2<int>;

/**
 * @brief Three-dimensional integer vector.
 */
using Vector3i = Vector3<int>;

/**
 * @brief Four-dimensional integer vector.
 */
using Vector4i = Vector4<int>;