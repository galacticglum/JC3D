#include <Platform/Windows/Win32Input.h>

#include <Application.h>
#include <GLFW/glfw3.h>

Input* Input::s_Instance = new Win32Input();
bool Win32Input::IsKeyPressed_Impl(const KeyCode keycode)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowContext());
	
	const int glfwKey = static_cast<int>(keycode);
	const int state = glfwGetKey(window, glfwKey);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Win32Input::IsMouseButtonPressed_Impl(const MouseButton button)
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowContext());
	
	const int glfwButton = static_cast<int>(button);
	const int state = glfwGetMouseButton(window, glfwButton);

	return state == GLFW_PRESS;
}

Vector2f Win32Input::GetMousePosition_Impl()
{
	GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindowContext());

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return Vector2f(static_cast<float>(x), static_cast<float>(y));
}
