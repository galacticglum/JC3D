#include <Application.h>

Application::Application()
{
	m_Window = std::unique_ptr<Window>(Window::Create());
}

Application::~Application() = default;

void Application::Run()
{
	while (m_IsRunning)
	{
		m_Window->OnUpdate();
	}
}
