#include <Math/Quaternion.h>

#include <Math/MathFunctions.h>
#include <cmath>

Vector<4, float> Quaternion::ToAxisAngle() const
{
	Vector<4, float> result;
	const float angle = 2.0f * std::acosf(W);
	const float length = sqrt(1.0f - angle * angle);

	const Vector<3, float> axis = Vector<3, float>(X, Y, Z) / length;
	result.X = axis.X;
	result.Y = axis.Y;
	result.Z = axis.Z;
	result.W = angle;

	return result;
}

Matrix4f Quaternion::ToMatrix() const
{
	Matrix4f matrix;

	matrix[0][0] = 1.0f - 2.0f * Y * Y - 2.0f* Z * Z;
	matrix[0][1] = 2.0f* X * Y + 2.0f* W * Z;
	matrix[0][2] = 2.0f * X * Z - 2.0f * W * Y;

	matrix[1][0] = 2.0f * X * Y - 2.0f * W * Z;
	matrix[1][1] = 1.0f - 2.0f * X * X - 2.0f * Z * Z;
	matrix[1][2] = 2.0f * Y * Z + 2.0f * W * X;

	matrix[2][0] = 2.0f * X * Z + 2.0f * W * Y;
	matrix[2][1] = 2.0f * Y * Z - 2.0f * W * X;
	matrix[2][2] = 1.0f - 2.0f * X * X - 2.0f * Y * Y;

	return matrix;
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-X, -Y, -Z, -W);
}

Quaternion operator*(const Quaternion& left, const Quaternion& right)
{
	const Vector<3, float> v1(right.X, right.Y, right.Z);
	const Vector<3, float> v2(left.X, left.Y, left.Z);

	const float w = right.W * left.W - Quaternion::Dot(right, left);
	const Vector<3, float> v = right.W * v2 + left.W * v1 + Vector<3, float>::Cross(v2, v1);

	return Quaternion(v.X, v.Y, v.Z, w);
}

Vector<3, float> operator*(const Quaternion& quaternion, const Vector<3, float>& vector)
{
	const float w2 = quaternion.W * quaternion.W;
	return (2.0f * w2 - 1.0f) * vector + 2.0f * Vector<3, float>::Dot(quaternion.GetAxis(), vector) 
		* quaternion.GetAxis() + w2 * Vector<3, float>::Cross(quaternion.GetAxis(), vector);
}

Quaternion operator+(const Quaternion& left, const Quaternion& right)
{
	return Quaternion(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
}

Quaternion operator*(const Quaternion& left, const float scalar)
{
	return Quaternion(scalar * left.X, scalar * left.Y, scalar * left.Z, scalar * left.W);
}

Quaternion operator*(const float scalar, const Quaternion& right)
{
	return Quaternion(scalar * right.X, scalar * right.Y, scalar * right.Z, scalar * right.W);
}

float Quaternion::Magnitude() const
{
	return std::sqrtf(SquareMagnitude());
}

float Quaternion::SquareMagnitude() const
{
	return W * W + X * X + Y * Y + Z * Z;
}

Quaternion Quaternion::Normalized() const
{
	Quaternion quaternion = Quaternion(*this);
	return Normalize(quaternion);
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(-X, -Y, -Z, W);
}

Vector<3, float> Quaternion::ToEulerAngles() const
{
	// roll (x-axis rotation)
	const float sinrcosp = 2.0 * (W * X + Y * Z);
	const float cosrosp = 1.0 - 2.0 * (X * X + Y * Y);
	
	float roll = std::atan2f(sinrcosp, cosrosp);

	// pitch (y-axis rotation)
	const float sinp = 2.0 * (W * Y - Z * X);
	float pitch = std::asinf(sinp);
	if (std::fabs(sinp) >= 1)
	{
		pitch = std::copysign(MathFunctions::PI_OVER_2, sinp);
	}

	// yaw (z-axis rotation)
	const float sinycosp = 2.0 * (W * Z + X * Y);
	const float cosycosp = 1.0 - 2.0 * (Y * Y + Z * Z);
	const float yaw = std:: atan2f(sinycosp, cosycosp);
	return Vector3f(roll, pitch, yaw);
}

Quaternion& Quaternion::Normalize(Quaternion& quaternion)
{
	const float magnitude = quaternion.Magnitude();
	quaternion = quaternion * (1.0f / magnitude);
	return quaternion;
}

float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
	return a.W * b.W + a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

Quaternion Quaternion::Lerp(const Quaternion& a, const Quaternion& b, float t, bool shortest)
{
	const Quaternion correctedStart = a.Normalized();
	Quaternion correctDestination = b;

	if (shortest && Dot(a, b) < 0)
	{
		correctDestination = -b;
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
		correctDestination = -b;
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

Quaternion Quaternion::FromEulerAngles(const Vector<3, float>& angles)
{
	const float yaw = angles.Z;
	const float pitch = angles.Y;
	const float roll = angles.X;

	const float cy = std::cosf(yaw * 0.5);
	const float sy = std::sinf(yaw * 0.5);
	const float cp = std::cosf(pitch * 0.5);
	const float sp = std::sinf(pitch * 0.5);
	const float cr = std::cosf(roll * 0.5);
	const float sr = std::sinf(roll * 0.5);

	Quaternion quaternion;
	quaternion.W = cy * cp * cr + sy * sp * sr;
	quaternion.X = cy * cp * sr - sy * sp * cr;
	quaternion.Y = sy * cp * sr + cy * sp * cr;
	quaternion.Z = sy * cp * cr - cy * sp * sr;

	return quaternion;
}

Vector<3, float> Quaternion::Rotate(const Quaternion& quaternion, const Vector<3, float>& vector)
{
	return quaternion * vector;
}

Quaternion Quaternion::Rotation(const Vector<3, float>& u, const Vector<3, float>& v)
{
	const float uv = std::sqrtf(Vector<3, float>::Dot(u, u) * Vector<3, float>::Dot(v, v));
	float real = uv + Vector<3, float>::Dot(u, v);
	Vector<3, float> w;

	if (real < 1.e-6f * uv)
	{
		// Rotate 180 around an arbitrary orthogonal axis.
		real = 0.0f;
		w = std::abs(u.X) > std::abs(u.Z) ? Vector<3, float>(-u.Y, u.X, 0.f) : Vector<3, float>(0.f, -u.Z, u.Y);
	}
	else
	{
		w = Vector<3, float>::Cross(u, v);
	}

	return Quaternion(w.X, w.Y, w.Z, real).Normalized();
}

std::ostream& operator<<(std::ostream& stream, const Quaternion& right)
{
	return stream << right.ToString();
}
