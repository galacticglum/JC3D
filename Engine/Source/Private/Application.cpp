#include <Application.h>

#include <Core.h>
#include <Events/Event.h>

Application::Application()
{
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT(Application::OnEvent));
}

Application::~Application() = default;

void Application::Run() const
{
	while (m_IsRunning)
	{
		m_Window->OnUpdate();

		// Update the layers.
		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate();
		}
	}
}

void Application::OnEvent(Event& event)
{
	EventDispather dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));

	Logger::Log(LoggerVerbosity::Trace, event);
	for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
	{
		(*--it)->OnEvent(event);
		if (event.Handled) break;
	}
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
	m_IsRunning = false;
	return true;
}
