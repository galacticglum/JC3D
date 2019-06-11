#include <Platform/OpenGL/OpenGLContext.h>
#include <Logger.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
	LOG_CATEGORY_ASSERT(windowHandle, "Renderer", "Window handle is null!");
}

void OpenGLContext::Initialize()
{
	glfwMakeContextCurrent(m_WindowHandle);
	const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	LOG_CATEGORY_ASSERT(gladStatus, "Renderer", "Failed to initialize Glad!");

	Logger::Log("Renderer", LoggerVerbosity::Info, "OpenGL Initialized:");
	Logger::Log("Renderer", LoggerVerbosity::Info, "  Vendor: {0}", glGetString(GL_VENDOR));
	Logger::Log("Renderer", LoggerVerbosity::Info, "  Renderer: {0}", glGetString(GL_RENDERER));
	Logger::Log("Renderer", LoggerVerbosity::Info, "  Version: {0}", glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
