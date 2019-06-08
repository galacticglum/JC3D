#include <iostream>
#include <Math/Vector.h>
#include <Math/Matrix.h>

int main(int argc, char* argv[])
{
	Matrix4f test;
	Matrix4f::Transpose(test);

	Vector3f vec;
	
    std::cout << "Hello World!" << std::endl;
    return 0;
}