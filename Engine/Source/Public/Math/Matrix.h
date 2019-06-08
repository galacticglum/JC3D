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
#include <string>
#include <sstream>

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
  * @struct MatrixBase Matrix.h
  * @brief A base generic matrix implementation supporting arbitrary element types.
  * @note Serves a wrapper around derived matrix classes.
  *
  * @tparam T the type of the matrix elements.
  * @tparam Derived The type of the derived matrix class.
  *			@p Derived is assumed to be a Matrix type (base or subclass of Matrix).
  * @tparam m The number of rows.
  * @tparam n The number of columns.
  */
template<std::size_t m, std::size_t n, typename T, typename Derived>
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
	 * The number of columns (n).
	 */
	const std::size_t Width = n;

	/**
	 * The number of rows (m).
	 */
	const std::size_t Height = m;

	/**
	 * @brief The size of this Matrix as a two-dimensional vector where the x-component
	 *		  represents the width and the y-component represents the height.
	 */
	const Vector2<std::size_t> Size = Vector2<std::size_t>(Width, Height);

	union
	{
		/**
		 * @brief The Matrix elements.
		 */
		T Data[n][m];

		struct
		{
			/**
			 * @brief The columns of this Matrix stored as m-dimensional vectors.
			 */
			Vector<m, T> Columns[n];
		};

		struct
		{
			/**
			 * @brief The rows of this Matrix stored as n-dimensional vectors.
			 */
			Vector<n, T> Rows[m];
		};
	};

	/**
	 * @brief Initializes this Matrix as an identity matrix.
	 * @param scalar The value to use for the diagonal. Defaults to 1.
	 */
	explicit MatrixBase(const T& scalar = 1)
	{
		// Initialize as identity matrix.
		for (std::size_t column = 0; column < n; ++column)
		{
			for (std::size_t row = 0; row < m; ++row)
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
		assert(args.size() <= m * n);
		std::size_t columns = 0;
		std::size_t rows = 0;

		for (auto& it : args)
		{
			Data[columns][rows++] = it;
			if (rows >= m)
			{
				++columns;
				rows = 0;
			}
		}
	}

	/**
	 * @brief Converts a @p scale vector to the scale-transformation matrix.
	 */
	static Matrix<n, n, T> Scale(const Vector<n, T>& scale)
	{
		Matrix<n, n, T> result;
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
	static Matrix<n, m, T> Transpose(const Matrix<m, n, T>& matrix)
	{
		Matrix<n, m, T> result;
		for (std::size_t column = 0; column < m; ++column)
		{
			for (std::size_t row = 0; row < n; ++row)
			{
				result[column][row] = matrix[row][column];
			}
		}

		return result;
	}

	/**
	 * @brief Gets the column Vector at the specified @p index.
	 * @param index The index of the column whose vector to retrieve.
	 * @returns A reference to the vector at the specified @p index.
	 */
	Vector<m, T>& GetColumn(const std::size_t index) const
	{
		assert(index >= 0 && index < n);
		return const_cast<Vector<m, T>&>(Columns[index]);
	}

	/**
	 * @brief Sets the column Vector at the specified @p index.
	 * @param index The index of the column whose vector to set to the specified @p column value.
	 * @param column An m-dimensional vector representing the new value of the column at the specified @p index.
	 */
	void SetColumn(const std::size_t index, const Vector<m, T>& column)
	{
		assert(index >= 0 && index < n);
		Columns[index] = column;
	}

	/**
	 * @brief Gets the row Vector at the specified @p index.
	 * @param index The index of the row whose vector to retrieve.
	 * @returns A reference to the vector at the specified @p index.
	 */
	Vector<n, T>& GetRow(const std::size_t index) const
	{
		assert(index >= 0 && index < m);
		return Rows[index];
	}

	/**
	 * @brief Sets the row Vector at the specified @p index.
	 * @param index The index of the row whose vector to set to the specified @p row value.
	 * @param row An n-dimensional vector representing the new value of the row at the specified @p index.
	 */
	void SetRow(const std::size_t index, const Vector<n, T>& row)
	{
		assert(index >= 0 && index < m);
		Rows[index] = row;
	}

	/**
	  * @brief Gets the column Vector at the specified @p index.
	  * @param index The index of the column whose vector to retrieve.
	  * @returns A reference to the vector at the specified @p columnIndex.
	  */
	Vector<m, T>& operator[](const std::size_t index)
	{
		return GetColumn(index);
	}

	/**
	 * @brief Gets the column Vector at the specified @p index.
	 * @param index The index of the column whose vector to retrieve.
	 * @returns A reference to the vector at the specified @p columnIndex.
	 */
	const Vector<m, T>& operator[](const std::size_t index) const
	{
		return GetColumn(index);
	}

	/**
	 * @brief Converts this Matrix to a std::string.
	 */
	std::string ToString() const
	{
		std::stringstream result;
		for (std::size_t row = 0; row < n; ++row)
		{
			result << Rows[row] << (row < n - 1 ? "\n" : "");
		}

		return result.str();
	}
};

/**
 * @struct Matrix Matrix.h
 * @brief A generic m by n matrix implementation.
 * @note The matrices are stored in column-major order. The
 *		 transformations also assume column-major order.
 * 
 */
MATRIX_TEMPLATE
struct Matrix : MatrixBase<m, n, T, Matrix<m, n, T>>
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
	explicit Matrix(const T& scalar) : MatrixBase<m, n, T, Matrix<m, n, T>>(scalar)
	{
	}

	/**
	 * @brief Initializes a new Matrix using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @note The elements go from the top-left corner of the Matrix and then go column-to-column for each row.
	 * @param args The std::initializer_list<T> used to initialize this Matrix's elements.
	 */
	Matrix(const std::initializer_list<T> args) : MatrixBase<m, n, T, Matrix<m, n, T>>(args)
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

/**
 * @brief Matrix string stream operator.
 */
MATRIX_TEMPLATE
std::ostream& operator<<(std::ostream& stream, const Matrix<m, n, T>& matrix)
{
	return (stream << matrix.ToString());
}

#include <Math/Matrix4.h>

template <MATRIX_TYPENAME_TEMPLATE>
/**
 * @brief A 4x4 matrix with arbitrary element type.
 */
using Matrix4 = Matrix<4, 4, T>;

/**
 * @brief A floating-point precision 4x4 matrix.
 */
using Matrix4f = Matrix4<float>;

/**
 * @brief A double-precision 4x4 matrix.
 */
using Matrix4d = Matrix4<double>;

/**
 * @brief An integer 4x4 matrix.
 */
using Matrix4i = Matrix4<int>;