#include <Platform/OpenGL/OpenGLContext.h>
#include <Logger.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
	LOG_CATEGORY_ASSERT(windowHandle, "Engine", "Window handle is null!");
}

void OpenGLContext::Initialize()
{
	glfwMakeContextCurrent(m_WindowHandle);
	const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	LOG_CATEGORY_ASSERT(gladStatus, "Engine", "Failed to initialize Glad!")
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
