/**
 * Author: Shon Verch
 * File Name: Matrix4.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/06/19
 * Modified Date: 06/06/19
 * Description: Specialized implementation of a square matrix
 *				with dimension four.
 */

#pragma once

#include <Math/Matrix.h>
#include <Math/MathFunctions.h>

#include <cmath>

 /**
  * @struct Matrix Matrix4.h "Engine/Source/Public/Math/Matrix4.h"
  * @brief A specialized implementation of a square matrix
  *		   with dimension four.
  * @tparam T The type of the vector elements.
  */
template<typename T>
struct Matrix<4, 4, T> : MatrixBase<4, 4, T, Matrix<4, 4, T>>
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
	explicit Matrix(const T& scalar) : MatrixBase<4, 4, T, Matrix<4, 4, T>>(scalar)
	{
	}

	/**
	 * @brief Initializes a new Matrix using an std::initializer_list<T> for accepting an arbitrary number of arguments.
	 * @note The elements go from the top-left corner of the Matrix and then go column-to-column for each row.
	 * @param args The std::initializer_list<T> used to initialize this Matrix's elements.
	 */
	Matrix(const std::initializer_list<T> args) : MatrixBase<4, 4, T, Matrix<4, 4, T>>(args)
	{
	}

	/**
	 * @brief Creates a translation matrix from a @p translation vector.
	 */
	static Matrix<4, 4, T> Translate(const Vector<3, T>& translation)
	{
		Matrix<4, 4, T> result;
		result[3] = Vector<4, T>(translation, 1);
		return result;
	}

	/**
	 * @brief Creates a scale matrix from a @p scale vector.
	 */
	static Matrix<4, 4, T> Scale(const Vector<3, T>& scale)
	{
		Matrix<4, 4, T> result;
		return Matrix<4, 4, T>::Scale(Vector<4, T>({ scale.X, scale.Y, scale.Z, 0 }));
	}

	/**
	 * @brief Creates a rotation matrix from @p eulerAngles.
	 * @param eulerAngles The angles (yaw, pitch, roll).
	 * @param radians A boolean indicating whether the angles are in radians or degrees.
	 *				  Defaults to false.
	 */
	static Matrix<4, 4, T> Rotate(const Vector<3, T>& eulerAngles, bool radians = false)
	{
		Matrix<4, 4, T> rx;
		Matrix<4, 4, T> ry;
		Matrix<4, 4, T> rz;

		T x = eulerAngles.X;
		T y = eulerAngles.Y;
		T z = eulerAngles.Z;

		if (!radians)
		{
			x = MathFunctions::DegreesToRadians(x);
			y = MathFunctions::DegreesToRadians(y);
			z = MathFunctions::DegreesToRadians(z);
		}

		rx[1][1] = static_cast<T>(std::cos(x));
		rx[1][2] = static_cast<T>(-std::sin(x));
		rx[2][1] = static_cast<T>(std::sin(x));
		rx[2][2] = static_cast<T>(std::cos(x));
											 
		ry[0][0] = static_cast<T>(std::cos(y));
		ry[0][2] = static_cast<T>(-std::sin(y));
		ry[2][0] = static_cast<T>(std::sin(y));
		ry[2][2] = static_cast<T>(std::cos(y));
											 
		rz[0][0] = static_cast<T>(std::cos(z));
		rz[0][1] = static_cast<T>(-std::sin(z));
		rz[1][0] = static_cast<T>(std::sin(z));
		rz[1][1] = static_cast<T>(std::cos(z));

		return rx * ry * rz;
	}

	/**
	 * @brief Creates a rotation matrix from a rotation about an arbitrary @p axis
	 *		  by some @p angle.
	 */
	static Matrix<4, 4, T> Rotate(const Vector<3, T>& axis, const T& angle)
	{
		Matrix<4, 4, T> result;

		const T c = static_cast<T>(std::cos(angle));
		const T s = static_cast<T>(std::sin(angle));
		const T t = static_cast<T>(1 - c);

		// Set upper 3x3 portion of the matrix
		// Set column 0
		result[0][0] = t * axis.X * axis.X + c;
		result[0][1] = t * axis.X * axis.Y + s * axis.Z;
		result[0][2] = t * axis.X * axis.Z - s * axis.Y;

		// Set column 1
		result[1][0] = t * axis.X * axis.Y - s * axis.Z;
		result[1][1] = t * axis.Y * axis.Y + c;
		result[1][2] = t * axis.Y * axis.Z + s * axis.X;

		// Set column 2		
		result[2][0] = t * axis.X * axis.Z + s * axis.Y;
		result[2][1] = t * axis.Y * axis.Z - s * axis.X;
		result[2][2] = t * axis.Z * axis.Z + c;

		return result;
	}

	/**
	 * @brief Creates a rotation matrix from a @p forward and @p up vector.
	 */
	static Matrix<4, 4, T> Rotate(const Vector<3, T>& forward, const Vector<3, T>& up)
	{
		Matrix<4, 4, T> result;
		Vector<3, T> n = Vector<3, T>::Normalize(forward);
		Vector<3, T> u = Vector<3, T>::Cross(up.Normalized(), n);
		Vector<3, T> v = Vector<3, T>::Cross(n, u);

		// Set row 0
		result[0][0] = u.X;
		result[1][0] = u.Y;
		result[2][0] = u.Z;
		result[3][0] = static_cast<T>(0);

		// Set row 1
		result[0][1] = v.X;
		result[1][1] = v.Y;
		result[2][1] = v.Z;
		result[3][1] = static_cast<T>(0);

		// Set row 2
		result[0][2] = n.X;
		result[1][2] = n.Y;
		result[2][2] = n.Z;
		result[3][2] = static_cast<T>(0);

		// Set row 3
		result[0][3] = static_cast<T>(0);
		result[1][3] = static_cast<T>(0);
		result[2][3] = static_cast<T>(0);
		result[3][3] = static_cast<T>(1);

		return result;
	}

	/**
	 * @brief Given a @p source vector, @p destination vector, and an @up vector, computes a transformation matrix
	 *		  corresponds to viewing the @p destination from the @p source, such that the right-hand vector is perpendicular
	 *		  to the @p up vector.
	 * @param source The source vector.
	 * @param destination The destination vector.
	 * @param up The vector describing the up direction.
	 * @returns The resulting transformation matrix.
	 */
	static Matrix<4, 4, T> LookAt(const Vector<3, T>& source, const Vector<3, T>& destination, const Vector<3, T>& up)
	{
		Matrix<4, 4, T> frame;
		Matrix<4, 4, T> translate;

		Vector<3, T> forward = Vector<3, T>::Normalize(destination - source);
		Vector<3, T> right = Vector<3, T>::Normalize(Vector<3, T>::Cross(forward, up));
		Vector<3, T> u = Vector<3, T>::Cross(right, forward);

		frame[0] = Vector<4, T>(right, frame[0].W);
		frame[1] = Vector<4, T>(u, frame[1].W);
		frame[2] = Vector<4, T>(-forward, frame[2].W);
		frame = Matrix<4, 4, T>::Transpose(frame);

		translate[3] = Vector<4, 4, T>(-source, translate[3].W);
		return frame * translate;
	}
};
