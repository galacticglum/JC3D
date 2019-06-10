#include <ImGui/ImGuiLayer.h>

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

#include <Application.h>
#include <Engine.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
{
}

ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::OnAttach()
{
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup styling
	ImGui::StyleColorsDark();

	// When we viewports are enabled, we teak WindowRounding/WindowBg so that platform windows
	// can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0;
		style.Colors[ImGuiCol_WindowBg].w = 1;
	}

	Application& application = Application::Get();
	GLFWwindow* window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindowContext());

	// Setup platform/renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::OnImGuiRender()
{
	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiLayer::Begin() const
{
	// Start a new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End() const
{
	// Update the window size
	ImGuiIO& io = ImGui::GetIO();
	Application& application = Application::Get();
	io.DisplaySize = ImVec2(application.GetWindow().GetWidth(), application.GetWindow().GetHeight());

	// Render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		// Cache our currently active viewport so that we can revert
		// back to it after ImGui renders its viewport.
		GLFWwindow* currentContext = glfwGetCurrentContext();

		// Render the active viewport
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		
		glfwMakeContextCurrent(currentContext);
	}
}
