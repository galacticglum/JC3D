#include <iostream>
#include <Logger.h>
#include <Window.h>

int main(int argc, char* argv[])
{
	const Window window("Window", 800, 600);

	while (!window.IsCloseRequested())
	{
		Window::Clear();
		window.Update();
	}

    return 0;
}