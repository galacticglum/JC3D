#include <Math/Quaternion.h>

#include <Math/MathFunctions.h>
#include <cmath>

Quaternion Quaternion::AxisAngle(const Vector<3, float>& axis, float angle, const bool radians)
{
	if (!radians)
	{
		angle = MathFunctions::DegreesToRadians(angle);
	}

	const float halfAngle = radians * 0.5f;
	return Quaternion(axis * std::sin(halfAngle),  std::cos(halfAngle));
}