#include <Window.h>
#include <utility>

#include <GL/glew.h>

/**
 * @brief Raised when the window is resized.
 * @param window The window context.
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
void WindowResizeCallback(GLFWwindow* window, int width, int height);
void WindowResizeCallback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}


Window::Window(std::string title, const int width, const int height) :
	m_Title(std::move(title)), m_Width(width), m_Height(height)
{
	if(!Initialize())
	{
		glfwTerminate();
	}
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::Initialize()
{
	if(!glfwInit())
	{
		return false;
	}

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	if (m_Window)
	{
		return false;
	}

	glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
	MakeContextCurrent();

	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	return true;
}

void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Window::SetTitle(const std::string& title)
{
	m_Title = title;
	glfwSetWindowTitle(m_Window, m_Title.c_str());
}




