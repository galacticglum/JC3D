/**
 * Author: Shon Verch
 * File Name: Vector2.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/04/19
 * Modified Date: 06/04/19
 * Description: Specialized implementation of a two-dimensional vector 
 *				supporting arbitrary types.
 */

#pragma once

#include <Math/Vector.h>

template<typename T>
struct Vector<2, T> : VectorBase<T, Vector<3, T>>
{
	union
	{
		
	};
};