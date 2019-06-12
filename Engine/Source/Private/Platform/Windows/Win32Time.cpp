#include <Platform/Windows/Win32Time.h>
#include <Logger.h>
#include <GLFW/glfw3.h>

double Win32Time::GetTime()
{
	LOG_CATEGORY_ASSERT(!GLFW_NOT_INITIALIZED, "Engine", "GLFW is not initialized!");
	return glfwGetTime();
}

Time* Time::Create()
{
	return new Win32Time();
}
