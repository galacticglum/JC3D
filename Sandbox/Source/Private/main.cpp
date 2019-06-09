#include <iostream>
#include <Utilities/Logger.h>

int main(int argc, char* argv[])
{
	Logger::Initialize();
	Logger::Log("", "test");

    std::cout << "Hello World!" << std::endl;
    return 0;
}