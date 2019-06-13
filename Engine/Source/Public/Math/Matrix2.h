/**
 * Author: Shon Verch
 * File Name: Matrix2.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/13/2019
 * Modified Date: 06/13/2019
 * Description: Specialized implementation of a square matrix
 *				with dimension three.
 */

#pragma once

#include <Math/Matrix.h>

 /**
  * @struct Matrix Matrix2.h
  * @brief A specialized implementation of a square matrix
  *		  width dimension three.
  * @tparam T The type of the matrix elements.
  */
template<typename T>
struct Matrix<2, 2, T> : MatrixBase<2, 2, T, Matrix<2, 2, T>>
{
	/**
	 * @brief A default empty constructor that initializes an identity matrix.
	 */
	Matrix() : Matrix(1)
	{
	}

	/**
	 * @brief Initialize this Matrix as an identity matrix with the specified
	 *		  @p scalar value as the diagonal.
	 * @note Implemented as multiplying the identity matrix by the @p scalar.
	 */
	explicit Matrix(const T& scalar) : MatrixBase<2, 2, T, Matrix<2, 2, T>>(scalar)
	{
	}

	/**
	 * @brief Initializes a new Matrix using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @note The elements should be specified in column-major order; that is, the first m elements denote the elements
	 *		 of the first column, the second m elements denote the elements of the second column, and so on until the n-th
	 *		 row.
	 * @param args The std::initializer_list<T> used to initialize this Matrix's elements.
	 */
	Matrix(const std::initializer_list<T> args) : MatrixBase<2, 2, T, Matrix<2, 2, T>>(args)
	{
	}

	/**
	 * @brief The determinant.
	 * @note Implemented as a Laplace expansion.
	 */
	T Determinant() const
	{
		return Data[0][0] * Data[1][1] - Data[1][0] * Data[0][1];
	}
};