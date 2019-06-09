#include <Platform/Windows/Win32Windows.h>
#include <Window.h>
#include <Logger.h>

static bool s_IsGLFWInitialized = false;

Window* Window::Create(const WindowProperties& props)
{
	return new Win32Windows(props);
}

Win32Windows::Win32Windows(const WindowProperties& props)
{
	Initialize(props);
}

Win32Windows::~Win32Windows()
{
	Win32Windows::Shutdown();
}

void Win32Windows::OnUpdate() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Win32Windows::ToggleVSync(const bool enabled)
{
	glfwSwapInterval(enabled ? 1 : 0);
	m_Data.IsVSyncEnabled = enabled;
}

void Win32Windows::Initialize(const WindowProperties& props)
{
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	Logger::Log("Engine", LoggerVerbosity::Info, "Create window {} ({} x {})", props.Title, props.Width, props.Height);
	if (!s_IsGLFWInitialized)
	{
		const int success = glfwInit();
		LOG_CATEGORY_ASSERT(success, "Engine", "Could not initialize GLFW!");
		s_IsGLFWInitialized = success;
	}

	m_Window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height), m_Data.Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);
	glfwSetWindowUserPointer(m_Window, &m_Data);
	ToggleVSync(true);
}

void Win32Windows::Shutdown()
{
	glfwDestroyWindow(m_Window);
}
