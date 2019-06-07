/**
 * Author: Shon Verch
 * File Name: Matrix.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/05/19
 * Modified Date: 06/05/19
 * Description: Generic m by n matrix implementation.
 */

#pragma once

#include <cassert>
#include <initializer_list>
#include <Math/Vector.h>

 /**
  * @brief Macro definition for the generic matrix template typename definition.
  * @note Restricts to arithmetic types only.
  */
#define MATRIX_TYPENAME_TEMPLATE typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type

  /**
   * @brief Macro definition for the generic matrix template definition including typename restriction to only numeric types.
   */
#define MATRIX_TEMPLATE template <std::size_t m, std::size_t n, MATRIX_TYPENAME_TEMPLATE>

MATRIX_TEMPLATE
struct Matrix;

 /**
  * @struct MatrixBase Matrix.h "Public/Math/Matrix.h"
  * @brief A base generic matrix implementation supporting arbitrary element types.
  * @note Serves a wrapper around derived matrix classes.
  *
  * @tparam T the type of the matrix elements.
  * @tparam Derived The type of the derived matrix class.
  *			@p Derived is assumed to be a Matrix type (base or subclass of Matrix).
  * @tparam m The number of rows.
  * @tparam n The number of columns.
  */
template<typename T, typename Derived>
struct MatrixBase
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
	 * @brief The number of columns.
	 */
	static constexpr std::size_t N = Derived::N;

	/**
	 * @brief The number of rows.
	 */
	static constexpr std::size_t M = Derived::M;

	union
	{
		/**
		 * @brief The Matrix elements.
		 */
		T Data[N][M];

		struct
		{
			/**
			 * @brief The columns of this Matrix stored as m-dimensional columns.
			 */
			Vector<M, T> Columns[N];
		};
	};

	/**
	 * @brief Initializes this Matrix as an identity matrix.
	 * @param scalar The value to use for the diagonal. Defaults to 1.
	 */
	explicit MatrixBase(const T& scalar = 1)
	{
		// Initialize as identity matrix.
		for (std::size_t column = 0; column < N; ++column)
		{
			for (std::size_t row = 0; row < M; ++row)
			{
				Data[column][row] = (column == row) ? static_cast<T>(scalar) : static_cast<T>(0);
			}
		}
	}

	/**
	 * @brief Initializes a new Matrix using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @note The elements go from the top-left corner of the Matrix and then go column-to-column for each row.
	 * @param args The std::initializer_list<T> used to initialize this Matrix's elements.
	 */
	MatrixBase(const std::initializer_list<T> args)
	{
		assert(args.size() <= M * N);
		std::size_t columns = 0;
		std::size_t rows = 0;

		for (auto& it : args)
		{
			Data[columns][rows++] = it;
			if (rows >= M)
			{
				++columns;
				rows = 0;
			}
		}
	}

	/**
	 * @brief Converts a @p scale vector to the scale-transformation matrix.
	 */
	static Matrix<N, N, T> Scale(const Vector<N, T>& scale)
	{
		Matrix<N, N, T> result;
		for (std::size_t i = 0; i < N; ++i)
		{
			result[i][i] = scale[i];
		}

		return result;
	}

	/**
	 * @brief Transposes the specified @p matrix.
	 * @returns A new transposed copy of the @p matrix.
	 */
	static Matrix<N, M, T> Transpose(const Matrix<M, N, T>& matrix)
	{
		Matrix<N, M, T> result;
		for (std::size_t column = 0; column < M; ++column)
		{
			for (std::size_t row = 0; row < N; ++row)
			{
				result[column][row] = matrix[row][column];
			}
		}

		return result;
	}

	/**
	  * @brief Gets the column Vector at the specified @p index.
	  * @param columnIndex The index of the column whose vector to retrieve.
	  * @returns A reference to the vector at the specified @p columnIndex.
	  */
	Vector<M, T>& operator[](const std::size_t columnIndex)
	{
		assert(columnIndex >= 0 && columnIndex < N);
		return Columns[columnIndex];
	}
};

/**
 * @struct Matrix Matrix.h "Public/Math/Matrix.h"
 * @brief A generic m by n matrix implementation.
 * @note The matrices are stored in column-major order. The
 *		 transformations also assume column-major order.
 * 
 */
MATRIX_TEMPLATE
struct Matrix : MatrixBase<T, Matrix<m, n, T>>
{
	/**
	 * @brief Initialize this Matrix as an identity matrix with the specified
	 *		  @p scalar value as the diagonal.
	 * @note Implemented as multiplying the identity matrix by the @p scalar.
	 */
	explicit Matrix(const T& scalar = 1) : MatrixBase<T, Matrix<m, n, T>>(scalar)
	{
	}

	/**
	 * @brief Initializes a new Matrix using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @note The elements go from the top-left corner of the Matrix and then go column-to-column for each row.
	 * @param args The std::initializer_list<T> used to initialize this Matrix's elements.
	 */
	Matrix(const std::initializer_list<T> args) : MatrixBase<T, Matrix<m, n, T>>(args)
	{
	}
};

/**
 * @brief Multiples two matrices.
 * @note The matrices can be of any dimension so long as
 *		 the number of columns, @p n, of the matrix @p left
 *		 is equal to the number of rows, @p n, of the matrix
 *		 @p right.
 *		 
 *	@note The result of the matrix multiplication is always
 *		  an @p m by @p o matrix (i.e. numbers of rows of @p left
 *		  by number of columns of @p right).
 *	@returns A new @p m by @p o matrix of type @p T.
 */
template<std::size_t m, std::size_t n, std::size_t o, MATRIX_TYPENAME_TEMPLATE>
Matrix<m, o, T> operator*(const Matrix<m, n, T>& left, const Matrix<n, o, T>& right)
{
	Matrix<m, o, T> result;
	for(std::size_t column = 0; column < o; ++column)
	{
		for(std::size_t row = 0; row < m; ++row)
		{
			T value = {};
			for (std::size_t j = 0; j < n; ++j)
			{
				value += left[j][row] * right[column][j];
			}

			result[column][row] = value;
		}
	}

	return result;
}

/**
 * @brief Multiples two matrices.
 * @note The matrices can be of any dimension so long as
 *		 the number of columns, @p n, of the matrix @p left
 *		 is equal to the number of rows, @p n, of the matrix
 *		 @p right.
 *
 *	@note The result of the matrix multiplication is always
 *		  an @p m by @p o matrix (i.e. numbers of rows of @p left
 *		  by number of columns of @p right).
 *	@returns A reference to the multiplied matrix.
 */
template<std::size_t m, std::size_t n, std::size_t o, MATRIX_TYPENAME_TEMPLATE>
Matrix<m, o, T>& operator*=(Matrix<m, n, T>& left, const Matrix<n, o, T>& right)
{
	left = left * right;
	return left;
}

/**
 * @brief Multiples left matrix by left scalar value.
 * @returns A new @p m by @p n matrix of type @p T.
 */
MATRIX_TEMPLATE
Matrix<m, n, T> operator*(const Matrix<m, n, T>& left, T scalar)
{
	Matrix<m, n, T> result;
	for (std::size_t column = 0; column < n; ++column)
	{
		for (std::size_t row = 0; row < m; ++row)
		{
			result[column][row] = left[column][row] * scalar;
		}
	}

	return result;
}

/**
 * @brief Multiples a matrix by a scalar value.
 * @returns A new @p m by @p n matrix of type @p T.
 */
MATRIX_TEMPLATE
Matrix<m, n, T> operator*(T scalar, const Matrix<m, n, T>& right)
{
	return right * scalar;
}

/**
 * @brief Vector-matrix multiplication.
 */
MATRIX_TEMPLATE
Vector<m, T> operator*(const Matrix<m, n, T>& left, const Vector<n, T>& right)
{
	Vector<m, T> result;
	for(std::size_t row = 0; row < m; ++row)
	{
		T value = {};
		for(std::size_t j = 0; j < n; ++j)
		{
			value += left[j][row] * right[j];
		}

		result[row] = value;
	}

	return result;
}