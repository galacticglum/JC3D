/**
 * Author: Shon Verch
 * File Name: main.cpp
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: Entry point for the sandbox application.
 */

#include <Logger.h>
#include <Layer.h>
#include <Application.h>
#include <ImGui/ImGuiLayer.h>
#include <Input.h>

#include <imgui.h>

class TestLayer : public Layer
{
public:
	TestLayer() : Layer("Test")
	{	
	}

	void OnUpdate() override
	{
		//Logger::Log(LoggerVerbosity::Info, "TestLayer::OnUpdate");
		if (Input::IsKeyPressed(KeyCode::Tab))
		{
			Logger::Log(LoggerVerbosity::Trace, "Tab key pressed.");
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Event& event) override
	{
		//Logger::Log(LoggerVerbosity::Info, "TestLayer::OnEvent: {0}", event);
	}
};

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
	}

	~Sandbox() = default;
};

/**
 * Entry point for the sandbox application.
 */
int main()
{
	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
}