#include <Math/Quaternion.h>
#include <Math/MathFunctions.h>

#include <cmath>

const Quaternion Quaternion::Identity = Quaternion();

/**
 * @brief Get the square magnitude of @p quaternion.
 */
float SquareMagnitude(const Quaternion& quaternion)
{
	return quaternion.X * quaternion.X + quaternion.Y * quaternion.Y +
		quaternion.Z * quaternion.Z + quaternion.W * quaternion.W;
}

/**
 * @brief Normalize @p quaternion.
 */
Quaternion Normalize(const Quaternion& quaternion)
{
	const float squareMagnitude = SquareMagnitude(quaternion);
	const float inverseMagnitude = 1.0f / std::sqrt(squareMagnitude);
	return quaternion * inverseMagnitude;
}

Vector<3, float> Quaternion::GetAxis() const
{
	const float x = 1.0f - W * W;

	// Make sure that we don't divide by zero.
	if (x < MathFunctions::Epsilon) return Vector3f::XAxis;

	return GetVectorPart() / (x * x);
}

float Quaternion::GetPitch() const
{
	float const y = 2 * (Y * Z + W * X);
	float const x = W * W - X * X - Y * Y + Z * Z;

	// Avoid atan(0, 0)
	if (x < MathFunctions::Epsilon && y < MathFunctions::Epsilon)
	{
		return 2 * std::atan2f(X, W);
	}

	return std::atan2f(y, x);
}

float Quaternion::GetYaw() const
{
	return std::asinf(std::clamp(-2 * (X * Z - W * Y), -1.0f, 1.0f));
}

float Quaternion::GetRoll() const
{
	return std::atan2(2 * (X * Y + W * Z), W * W + X * X - Y * Y - Z * Z);
}

Vector<3, float> Quaternion::ToEulerAngles() const
{
	return Vector<3, float>(GetPitch(), GetYaw(), GetRoll());
}

Quaternion Quaternion::operator+(const Quaternion& quaternion) const
{
	return Quaternion(X + quaternion.X, Y + quaternion.Y, Z + quaternion.Z, W + quaternion.W);
}

Quaternion Quaternion::operator-(const Quaternion& quaternion) const
{
	return Quaternion(X - quaternion.X, Y - quaternion.Y, Z - quaternion.Z, W - quaternion.W);
}

Quaternion Quaternion::operator*(const Quaternion& quaternion) const
{
	return Normalize(Quaternion(
		W * quaternion.X + X * quaternion.W + Y * quaternion.Z - Z * quaternion.Y,
		W * quaternion.Y + Y * quaternion.W + Z * quaternion.X - X * quaternion.Z,
		W * quaternion.Z + Z * quaternion.W + X * quaternion.Y - Y * quaternion.X,
		W * quaternion.W - X * quaternion.X - Y * quaternion.Y - Z * quaternion.Z
	));
}

Quaternion Quaternion::operator*(const float scalar) const
{
	return Quaternion(X * scalar, Y * scalar, Z * scalar, W * scalar);
}

Quaternion Quaternion::operator/(const float scalar) const
{
	return Quaternion(X / scalar, Y / scalar, Z / scalar, W / scalar);
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-X, -Y, -Z, -W);
}

bool Quaternion::operator==(const Quaternion& quaternion) const
{
	return X == quaternion.X && Y == quaternion.Y && Z == quaternion.Z && W == quaternion.W;
}

bool Quaternion::operator!=(const Quaternion& quaternion) const
{
	return !(*this == quaternion);
}

Quaternion Quaternion::FromEulerAngles(const Vector<3, float>& angles)
{
	const float cx = std::cosf(angles.X * 0.5f);
	const float cy = std::cosf(angles.Y * 0.5f);
	const float cz = std::cosf(angles.Z * 0.5f);

	const float sx = std::sin(angles.X * 0.5f);
	const float sy = std::sin(angles.Y * 0.5f);
	const float sz = std::sin(angles.Z * 0.5f);

	Quaternion result;
	result.W = cx * cy * cz + sx * sy * sz;
	result.X = sx * cy * cz - cx * sy * sz;
	result.Y = cx * sy * cz + sx * cy * sz;
	result.Z = cx * cy * sz - sx * sy * cz;

	return result;
}

Vector<3, float> Quaternion::Rotate(const Quaternion& quaternion, const Vector<3, float>& vector)
{
	const float rx = quaternion.W * vector.X + quaternion.Y * vector.Z - quaternion.Z * vector.Y;
	const float ry = quaternion.W * vector.Y + quaternion.Z * vector.X - quaternion.X * vector.Z;
	const float rz = quaternion.W * vector.Z + quaternion.X * vector.Y - quaternion.Y * vector.X;
	const float rw = quaternion.X * vector.X + quaternion.Y * vector.Y + quaternion.Z * vector.Z;

	return Vector<3, float>(
		rw * quaternion.X + rx * quaternion.W - ry * quaternion.Z + rz * quaternion.Y,
		rw * quaternion.Y + ry * quaternion.W - rz * quaternion.X + rx * quaternion.Z,
		rw * quaternion.Z + rz * quaternion.W - rx * quaternion.Y + ry * quaternion.X
	);
}

Quaternion Quaternion::Rotation(const Vector<3, float>& a, const Vector<3, float>& b)
{
	const float cosHalfAngleX2 = std::sqrt(2.0f * (1.0f + Vector<3,float>::Dot(a, b)));
	const float recipCosHalfAngleX2 = 1.0f / cosHalfAngleX2;

	return Quaternion(Vector<3, float>::Cross(a, b) * recipCosHalfAngleX2, cosHalfAngleX2 * 0.5f);
}

Quaternion Quaternion::Rotation(const float radians, const Vector<3, float>& axis)
{
	const float angle = radians * 0.5f;
	return Quaternion(axis * sin(angle), cos(angle));
}

float Quaternion::Dot(const Quaternion& other) const
{
	return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-X, -Y, -Z, W);
}
