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

class TestLayer : public Layer
{
public:
	TestLayer() : Layer("Test")
	{	
	}

	void OnUpdate() override
	{
		Logger::Log(LoggerVerbosity::Info, "TestLayer::OnUpdate");
	}

	void OnEvent(const Event& event) override
	{
		Logger::Log(LoggerVerbosity::Info, "TestLayer::OnEvent: {0}", event);
	}
};

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new TestLayer());
		PushOverlay(new ImGuiLayer());
	}

	~Sandbox()
	{
		
	}
};

/**
 * Entry point for the sandbox application.
 */
int main(int argc, char** argv)
{
	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
}