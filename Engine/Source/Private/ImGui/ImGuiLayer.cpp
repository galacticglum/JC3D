#include <ImGui/ImGuiLayer.h>

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <Platform/OpenGL/ImGuiOpenGLRenderer.h>
#include <Application.h>
#include <Engine.h>

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
{
}

ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::OnAttach()
{
	// Setup ImGui
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	// Stolen from imgui_impl_glfw.cpp
	// TODO: Use a more generic, non GLFW specific, keycode system
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	// Initialize the ImGui OpenGL version with GLSL version 410
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{
}

void ImGuiLayer::OnUpdate()
{
	ImGuiIO& io = ImGui::GetIO();
	Application& application = Application::Get();
	io.DisplaySize = ImVec2(application.GetWindow().GetWidth(), application.GetWindow().GetHeight());

	// Track the delta time
	const float deltaTime = static_cast<float>(glfwGetTime());
	io.DeltaTime = m_DeltaTime > 0 ? (deltaTime - m_DeltaTime) : 1 / 60.0f;
	m_DeltaTime = deltaTime;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	// Show a demo window for now...
	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& event)
{
	EventDispather dispatcher(event);

	// Bind our event handlers to the the dispatcher.
	// The dispatcher will invoke the event handler
	// if the current event matches the type of the handler.
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT(ImGuiLayer::OnMouseButtonPressedEvent));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT(ImGuiLayer::OnMouseButtonReleasedEvent));
	dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT(ImGuiLayer::OnMouseMovedEvent));
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT(ImGuiLayer::OnMouseScrolledEvent));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(ImGuiLayer::OnKeyPressedEvent));
	dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT(ImGuiLayer::OnKeyTypedEvent));
	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT(ImGuiLayer::OnKeyReleasedEvent));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(ImGuiLayer::OnWindowResizedEvent));
}

bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event) const
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[static_cast<int>(event.GetMouseButton())] = true;

	return false;
}

bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) const
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[static_cast<int>(event.GetMouseButton())] = false;

	return false;
}

bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& event) const
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(event.Position.X, event.Position.Y);

	return false;
}

bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& event) const
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += event.Offset.X;
	io.MouseWheel += event.Offset.Y;

	return false;
}

bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event) const
{
	ImGuiIO& io = ImGui::GetIO();

	io.KeysDown[static_cast<int>(event.GetKeyCode())] = true;
	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	return false;
}

bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event) const
{
	ImGuiIO& io = ImGui::GetIO();
	const int keycode = static_cast<int>(event.GetKeyCode());
	if (keycode > 0 && keycode < 0x10000)
	{
		io.AddInputCharacter(static_cast<unsigned short>(keycode));
	}

	return false;
}

bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event) const
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[static_cast<int>(event.GetKeyCode())] = false;

	return false;
}

bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& event) const
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	glViewport(0, 0, event.GetWidth(), event.GetHeight());
	
	return false;
}
