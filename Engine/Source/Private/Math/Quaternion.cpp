#include <Math/Quaternion.h>

#include <Math/MathFunctions.h>
#include <cmath>

Vector<3, float> Quaternion::GetXYZ() const
{
	return Vector<3, float>(X, Y, Z);
}

Quaternion& Quaternion::SetXYZ(const Vector<3, float>& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
	return *this;
}

float Quaternion::Magnitude() const
{
	return std::sqrtf(SquareMagnitude());
}

float Quaternion::SquareMagnitude() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

Quaternion Quaternion::Negative() const
{
	return Quaternion(-X, -Y, -Z, -W);
}

Quaternion Quaternion::Normalized() const
{
	const float length = Magnitude();
	return Quaternion(X / length, Y / length, Z / length, W / length);
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-X, -Y, -Z, W);
}

Vector<3, float> Quaternion::GetAxis() const
{
	const float x = 1 - W * W;

	// Check so we don't divide by zero!
	if (x < MathFunctions::Epsilon)
	{
		return Vector<3, float>::XAxis;
	}

	return GetXYZ() / (x * x);
}

Vector<3, float> Quaternion::ToEulerAngles() const
{
	const float x1 = 2 * X * W - 2 * Y * Z;
	const float x2 = 1 - 2 * X * X - 2 * Z * Z;
	const float y1 = 2 * Y * W - 2 * X * Z;
	const float y2 = 1 - 2 * Y * Y - 2 * Z * Z;
	const float z = 2 * X * Y + 2 * Z * W;

	return Vector<3, float>(std::atan2f(x1, x2), std::atan2f(y1, y2), std::asinhf(z));
}

#define QUATERNION_COMPONENT_WISE_OP_COPY(x, op) (Quaternion(X op x.X, Y op x.Y, Z op x.Z, W op x.W))
#define QUATERNION_SCALAR_OP_COPY(x, op) (Quaternion(X op x, Y op x, Z op x, W op x))

Quaternion Quaternion::operator+(const Quaternion& quaternion) const
{
	return QUATERNION_COMPONENT_WISE_OP_COPY(quaternion, +);
}

Quaternion Quaternion::operator-(const Quaternion& quaternion) const
{
	return QUATERNION_COMPONENT_WISE_OP_COPY(quaternion, -);
}

Quaternion Quaternion::operator*(const Quaternion& quaternion) const
{
	return Quaternion(
		(((W * quaternion.X) + (X * quaternion.W)) + (Y * quaternion.Z)) - (Z * quaternion.Y),
		(((W * quaternion.Y) + (Y * quaternion.W)) + (Z * quaternion.X)) - (X * quaternion.Z),
		(((W * quaternion.Z) + (Z * quaternion.W)) + (X * quaternion.Y)) - (Y * quaternion.X),
		(((W * quaternion.W) - (X * quaternion.X)) - (Y * quaternion.Y)) - (Z * quaternion.Z)
	);
}

Quaternion Quaternion::operator*(const float scalar) const
{
	return QUATERNION_SCALAR_OP_COPY(scalar, *);
}

Quaternion Quaternion::operator/(const float scalar) const
{
	return QUATERNION_SCALAR_OP_COPY(scalar, /);
}

float Quaternion::operator[](const uint32_t index) const
{
	LOG_ASSERT(index >= 0 && index < 4, "Index out of bounds!");
	switch (index)
	{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		case 3: return W;
	}

	// We should never reach here due to the assert
	return 0;
}

Quaternion Quaternion::operator-() const
{
	return this->Negative();
}

bool Quaternion::operator==(const Quaternion& quaternion) const
{
	return (X == quaternion.X) && (Y == quaternion.Y) && (Z == quaternion.Z) && (W == quaternion.W);
}

bool Quaternion::operator!=(const Quaternion& quaternion) const
{
	return !(*this == quaternion);
}

Vector<3, float> Quaternion::Rotate(const Quaternion& quaternion, const Vector<3, float>& vector)
{
	const float x = (((quaternion.W * vector.X) + (quaternion.Y * vector.Z)) - (quaternion.Z * vector.Y));
	const float y = (((quaternion.W * vector.Y) + (quaternion.Z * vector.X)) - (quaternion.X * vector.Z));
	const float z = (((quaternion.W * vector.Z) + (quaternion.X * vector.Y)) - (quaternion.Y * vector.X));
	const float w = (((quaternion.X * vector.X) + (quaternion.Y * vector.Y)) + (quaternion.Z * vector.Z));

	return Vector<3, float>(
		((((w * quaternion.X) + (x * quaternion.W)) - (y * quaternion.Z)) + (z * quaternion.Y)),
		((((w * quaternion.Y) + (y * quaternion.W)) - (z * quaternion.X)) + (x * quaternion.Z)),
		((((w * quaternion.Z) + (z * quaternion.W)) - (x * quaternion.Y)) + (y * quaternion.X))
	);
}

Quaternion& Quaternion::Normalize(Quaternion& quaternion)
{
	quaternion /= quaternion.Magnitude();
	return quaternion;
}

float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
	float result = (a.X * b.X);
	result += (a.Y * b.Y);
	result += (a.Z * b.Z);
	result += (a.W * b.W);

	return result;
}

Quaternion Quaternion::FromEulerAngles(const Vector<3, float>& angles)
{
	const Quaternion pitch(Vector3f::XAxis, angles.X);
	const Quaternion yaw(Vector3f::YAxis, angles.Y);
	const Quaternion roll(Vector3f::ZAxis, angles.Z);

	return pitch * yaw * roll;
}

Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t, bool shortest)
{
	const Quaternion correctedStart = a.Normalized();
	Quaternion correctDestination = b;

	if (shortest && Dot(a, b) < 0)
	{
		correctDestination = b.Negative();
	}

	return Quaternion(MathFunctions::Lerp(correctedStart.X, correctDestination.X, t),
		MathFunctions::Lerp(correctedStart.Y, correctDestination.Y, t),
		MathFunctions::Lerp(correctedStart.Z, correctDestination.Z, t),
		MathFunctions::Lerp(correctedStart.W, correctDestination.W, t));
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t, const bool shortest)
{
	float destinationDot = Dot(b, b);
	Quaternion correctDestination = b;

	if (shortest && destinationDot < 0)
	{
		destinationDot = -destinationDot;
		correctDestination = b.Negative();
	}

	if (std::abs(destinationDot) >= 1 - MathFunctions::Epsilon)
	{
		return Lerp(a, correctDestination, t, false);
	}

	const float sqrtDest = std::sqrtf(1.0f - destinationDot * destinationDot);
	const float angle = std::atan2f(sqrtDest, destinationDot);
	const float invertedDest = 1.0f / sqrtDest;

	const float factor = std::sinf((1.0f - t) * angle) * invertedDest;
	const float destinationFactor = std::sinf(t * angle) * invertedDest;

	return (a * factor) + (correctDestination * destinationFactor);
}

std::string Quaternion::ToString() const
{
	return "(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ", " + std::to_string(W) + ")";
}

std::ostream& operator<<(std::ostream& stream, const Quaternion& right)
{
	return stream << right.ToString();
}

Quaternion Quaternion::Rotation(const Vector<3, float>& directionA, const Vector<3, float>& directionB)
{
	const float cosHalfAngleX2 = std::sqrt((2.0f * (1.0f + Vector3f::Dot(directionA, directionB))));
	const float reciprocalCosHlfAngleX2 = 1.0f / cosHalfAngleX2;
	return Quaternion(Vector3f::Cross(directionA, directionB) * reciprocalCosHlfAngleX2, cosHalfAngleX2 * 0.5f);
}

Quaternion Quaternion::Rotation(const Vector<3, float>& axis, float angle, const bool radians)
{
	if (!radians)
	{
		angle = MathFunctions::DegreesToRadians(angle);
	}

	const float halfAngle = radians * 0.5f;
	return Quaternion(axis * std::sin(halfAngle),  std::cos(halfAngle));
}
