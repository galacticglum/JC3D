#include <iostream>
#include <Math/Vector.h>
#include <Math/Matrix.h>

int main(int argc, char* argv[])
{
	Matrix4f test;
	test[0][test.Width - 1] = 1;

	std::cout << test << "\n#\n";
	std::cout << Matrix4f::Transpose(test) << "\n";

    std::cout << "Hello World!" << std::endl;
    return 0;
}