#include <Application.h>

#include <Common.h>
#include <Engine.h>

#include <Events/Event.h>
#include <Renderer/BufferLayout.h>

/**
 * @briefs Converts a ShaderDataType to an OpenGL data type.
 */
static GLenum ShaderDataTypeToGLType(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		case ShaderDataType::Vector2f: return GL_FLOAT;
		case ShaderDataType::Vector3f: return GL_FLOAT;
		case ShaderDataType::Vector4f: return GL_FLOAT;
		case ShaderDataType::Vector2i: return GL_INT;
		case ShaderDataType::Vector3i: return GL_INT;
		case ShaderDataType::Vector4i: return GL_INT;
		case ShaderDataType::Matrix3f: return GL_FLOAT;
		case ShaderDataType::Matrix4f: return GL_FLOAT;
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown ShaderDataType!");
	return 0;
}

Application* Application::s_Instance = nullptr;

Application::Application()
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

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.23f, 0.24f, 0, 1.0f,
		0.5f, -0.5f, 0.0f, 0.2f, 1, 0, 1.0f,
		0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
	{
		const BufferLayout layout = {
			{ ShaderDataType::Vector3f, "a_Position" },
			{ ShaderDataType::Vector4f, "a_Colour" }
		};

		m_VertexBuffer->SetLayout(layout);
	}

	uint32_t index = 0;
	const BufferLayout& layout = m_VertexBuffer->GetLayout();
	for (const BufferElement& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToGLType(element.Type), 
			element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.Offset));

		index++;
	}

	uint32_t indices[3] = {
		0, 1, 2
	};

	m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	const std::string vertexSource = R"(
		#version 330 core
		
		layout (location = 0) in vec3 a_Position;
		layout (location = 1) in vec4 a_Colour;

		out vec3 v_Position;
		out vec4 v_Colour;
		
		void main()
		{
			gl_Position = vec4(a_Position, 1);
			v_Position = a_Position;
			v_Colour = a_Colour;
		}
	)";

	const std::string fragmentSource = R"(
		#version 330 core

		layout (location = 0) out vec4 colour;
		in vec3 v_Position;
		in vec4 v_Colour;

		void main()
		{
			//colour = vec4(v_Position * 0.5 + 0.5, 1);
			colour = v_Colour;
		}
	)";

	m_Shader.reset(new Shader(vertexSource, fragmentSource));
}

Application::~Application() = default;

void Application::Run() const
{
	while (m_IsRunning)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();
		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
}

void Application::OnEvent(Event& event)
{
	EventDispather dispatcher(event);

	// Bind the WindowCloseEvent handler
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));

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

bool Application::OnWindowClose(WindowCloseEvent& event)
{
	m_IsRunning = false;
	return true;
}
