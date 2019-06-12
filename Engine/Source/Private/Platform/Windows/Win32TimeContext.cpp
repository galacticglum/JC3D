#include <Platform/Windows/Win32TimeContext.h>
#include <Logger.h>

#include <GLFW/glfw3.h>

double Win32TimeContext::GetTime()
{
	LOG_CATEGORY_ASSERT(!GLFW_NOT_INITIALIZED, "Engine", "GLFW is not initialized!");
	return glfwGetTime();
}

TimeContext* TimeContext::Create()
{
	return new Win32TimeContext();
}