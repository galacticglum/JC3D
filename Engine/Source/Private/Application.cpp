#include <Application.h>

#include <Core.h>
#include <Events/Event.h>
#include <glad/glad.h>

Application* Application::s_Instance = nullptr;

Application::Application()
{
	LOG_CATEGORY_ASSERT(!s_Instance, "Engine", "Application already exists!");
	s_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT(Application::OnEvent));
}

Application::~Application() = default;

void Application::Run() const
{
	while (m_IsRunning)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Update the layers.
		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate();
		}

		m_Window->OnUpdate();
	}
}

void Application::OnEvent(Event& event)
{
	EventDispather dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));

	for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
	{
		(*--it)->OnEvent(event);
		if (event.Handled) break;
	}
}

void Application::PushLayer(Layer* layer)
{
	layer->OnAttach();
	m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
	layer->OnAttach();
	m_LayerStack.PushOverlay(layer);
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
	m_IsRunning = false;
	return true;
}
