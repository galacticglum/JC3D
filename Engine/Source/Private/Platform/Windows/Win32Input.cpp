#include <Platform/Windows/Win32Input.h>

#include <Application.h>
#include <GLFW/glfw3.h>

Input* Input::s_Instance = new Win32Input();

bool Win32Input::IsKeyPressed_Impl(const int keycode)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowContext());
	const int state = glfwGetKey(window, keycode);

	return state == GLFW_PRESS || GLFW_REPEAT;
}

bool Win32Input::IsMouseButtonPressed_Impl(const int button)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowContext());
	const int state = glfwGetMouseButton(window, button);

	return state == GLFW_PRESS;
}

Vector2f Win32Input::GetMousePosition_Impl()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowContext());

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return Vector2f(static_cast<float>(x), static_cast<float>(y));
}
