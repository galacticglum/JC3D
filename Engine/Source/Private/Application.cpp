#include <Application.h>

#include <Common.h>
#include <Engine.h>

#include <Events/Event.h>

#include <Renderer/Renderer.h>
#include <Renderer/Framebuffer.h>

#include <glad/glad.h>
#include <imgui.h>

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

	// Initialize the renderer
	Renderer::Initialize();

	// Initialize the time
	m_TimeContext = std::unique_ptr<TimeContext>(TimeContext::Create());
}

Application::~Application() = default;

void Application::Run()
{
	OnInitialize();

	float lastFrame = 0;
	while (m_IsRunning)
	{
		const float currentFrame = static_cast<float>(m_TimeContext->GetTime());
		m_DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// We only render if the screen is not minimized
		if (!m_Minimized)
		{
			// Update the layers.
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			// Render ImGui
			Application* application = this;
			ENGINE_RENDER_1(application, {
				application->RenderImGui();
			});

			Renderer::WaitAndRender();
		}

		// Update the window
		m_Window->OnUpdate();
	}

	OnShutdown();
}

void Application::RenderImGui()
{
	m_ImGuiLayer->Begin();

	ImGui::Begin("Renderer");
	auto& renderApiCapabilities = RendererAPI::GetCapabilities();
	ImGui::Text("Vendor: %s", renderApiCapabilities.Vendor.c_str());
	ImGui::Text("Renderer: %s", renderApiCapabilities.Renderer.c_str());
	ImGui::Text("Version: %s", renderApiCapabilities.Version.c_str());
	ImGui::End();

	for (Layer* layer : m_LayerStack)
	{
		layer->OnImGuiRender();
	}

	m_ImGuiLayer->End();
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

	// Resize the viewport
	ENGINE_RENDER_2(width, height, {
		glViewport(0, 0, width, height);
	});

	// Resize all framebuffers
	auto& framebuffers = FramebufferPool::GetGlobal()->GetAll();
	for (auto& framebuffer : framebuffers)
	{
		framebuffer->Resize(width, height);
	}

	return false;
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
	m_IsRunning = false;
	return true;
}
