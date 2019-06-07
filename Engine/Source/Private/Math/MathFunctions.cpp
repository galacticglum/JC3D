#include <Math/MathFunctions.h>
#include <limits>

const float MathFunctions::Epsilon = std::numeric_limits<float>::epsilon();
const long MathFunctions::MaxLong = std::numeric_limits<long>::max();
const int MathFunctions::MaxInt = std::numeric_limits<int>::max();
const float MathFunctions::MaxFloat = std::numeric_limits<float>::max();
const double MathFunctions::MaxDouble = std::numeric_limits<double>::max();

long MathFunctions::Factorial(int n)
{
	long result = 1;
	for (; n > 1; n--)
	{
		result *= n;
	}

	return result;
}

long MathFunctions::BinomialCoefficient(const int n, const int k)
{
	return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

double MathFunctions::InverseSqrtFast(double x)
{
	const auto halfX = 0.5f * static_cast<float>(x);
	auto i = *reinterpret_cast<int*>(&x);

	i = 0x5f375a86 - (i >> 1);
	x = *reinterpret_cast<float*>(&i);
	x *= 1.5f - halfX * x * x;
	
	return x;
}
