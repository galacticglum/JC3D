#include <Platform/Windows/Win32Window.h>
#include <Window.h>
#include <Logger.h>
#include <Math/Vector.h>

#include <Events/ApplicationEvent.h>
#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>

#include <glad/glad.h>

static bool s_IsGLFWInitialized = false;

/**
 * @brief Handle any GLFW error.
 */
static void GLFWErrorCallback(const int error, const char* description)
{
	Logger::Log("Engine", LoggerVerbosity::Error, "GLFW Error ({0}): {1}", error, description);
}

Window* Window::Create(const WindowProperties& props)
{
	return new Win32Window(props);
}

Win32Window::Win32Window(const WindowProperties& props) : m_Window(nullptr)
{
	Win32Window::Initialize(props);
}

Win32Window::~Win32Window()
{
	Win32Window::Shutdown();
}

void Win32Window::OnUpdate() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Win32Window::ToggleVSync(const bool enabled)
{
	glfwSwapInterval(enabled ? 1 : 0);
	m_Data.IsVSyncEnabled = enabled;
}

void Win32Window::Initialize(const WindowProperties& props)
{
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	Logger::Log("Engine", LoggerVerbosity::Info, "Create window {} ({} x {})", props.Title, props.Width, props.Height);
	if (!s_IsGLFWInitialized)
	{
		const int success = glfwInit();
		LOG_CATEGORY_ASSERT(success, "Engine", "Could not initialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);
		s_IsGLFWInitialized = success;
	}

	m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height), m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);

	const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	LOG_CATEGORY_ASSERT(gladStatus, "Engine", "Failed to initialize Glad!")

	glfwSetWindowUserPointer(m_Window, &m_Data);
	ToggleVSync(true);

	InitializeEvents();
}


void Win32Window::InitializeEvents() const
{
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const int width, const int height)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		// Update window data
		data.Width = width;
		data.Height = height;

		// Create and dispatch event
		WindowResizeEvent resizeEvent(width, height);
		data.Handler(resizeEvent);
	});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		WindowCloseEvent closeEvent;
		data.Handler(closeEvent);
	});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, const int key, int scancode, const int action, int mods)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent keyPressedEvent(static_cast<KeyCode>(key), 0);
			data.Handler(keyPressedEvent);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent keyReleasedEvent(static_cast<KeyCode>(key));
			data.Handler(keyReleasedEvent);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent keyPressedEvent(static_cast<KeyCode>(key), 1);
			data.Handler(keyPressedEvent);
			break;
		}
		}
	});

	glfwSetCharCallback(m_Window, [](GLFWwindow* window, const unsigned int keycode)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		KeyTypedEvent keyTypedEvent(static_cast<KeyCode>(keycode));
		data.Handler(keyTypedEvent);
	});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, const int button, const int action, const int mods)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent mouseButtonPressedEvent(static_cast<MouseButton>(button));
			data.Handler(mouseButtonPressedEvent);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent mouseButtonReleasedEvent(static_cast<MouseButton>(button));
			data.Handler(mouseButtonReleasedEvent);
		}
		}
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const double xOffset, const double yOffset)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		MouseScrolledEvent mouseScrolledEvent(Vector2f(static_cast<float>(xOffset), static_cast<float>(yOffset)));
		data.Handler(mouseScrolledEvent);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, const double x, const double y)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		MouseMovedEvent mouseMovedEvent(Vector2f(static_cast<float>(x), static_cast<float>(y)));
		data.Handler(mouseMovedEvent);
	});
}

void Win32Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
}
