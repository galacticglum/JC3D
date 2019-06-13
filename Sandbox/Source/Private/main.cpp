/**
 * Author: Shon Verch
 * File Name: main.cpp
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/12/2019
 * Description: Entry point for the sandbox application.
 */

#include <Application.h>
#include <PBRLayer.h>

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new PBRLayer());
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