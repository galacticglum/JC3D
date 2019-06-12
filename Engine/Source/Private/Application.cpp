#include <Application.h>

#include <Common.h>
#include <Engine.h>

#include <Events/Event.h>
#include <Renderer/BufferLayout.h>

#include <glad/glad.h>

Application* Application::s_Instance = nullptr;

Application::Application() : m_DeltaTime(0)
{
	// Ensure that there is only application instance
	LOG_CATEGORY_ASSERT(!s_Instance, "Engine", "Application already exists!");
	s_Instance = this;

	// Create and initialize the window
	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT(Application::OnEvent));

	// Initialize the ImGuiLayer instance
	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	// Initialize the time
	m_Time = std::unique_ptr<Time>(Time::Create());
}

Application::~Application() = default;

void Application::Run()
{
	OnInitialize();

	float lastFrame = 0;
	while (m_IsRunning)
	{
		const float currentFrame = static_cast<float>(m_Time->GetTime());
		m_DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update the layers.
		for (Layer* layer : m_LayerStack)
		{
			layer->OnUpdate();
		}

		// Render ImGui
		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}

		m_ImGuiLayer->End();

		// Update the window
		m_Window->OnUpdate();
	}

	OnShutdown();
}

void Application::OnEvent(Event& event)
{
	EventDispather dispatcher(event);

	// Bind window event handlers
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(Application::OnWindowResize));

	// Dispatch events on our layers
	for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
	{
		(*--it)->OnEvent(event);

		// If our layer handles an event, lets break out of this loop
		// since we don't want any layers below to also receive the event.
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

bool Application::OnWindowResize(WindowResizeEvent& event)
{
	const int width = event.GetWidth();
	const int height = event.GetHeight();
	if (width == 0 || height == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
	return true;
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
	m_IsRunning = false;
	return true;
}
