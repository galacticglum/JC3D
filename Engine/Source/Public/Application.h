/**
 * Author: Shon Verch
 * File Name: Application.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: Main application class.
 */

#pragma once

#include <memory>
#include <Window.h>

/**
 * @class Application Application.h
 */
class Application
{
public:
	Application();
	virtual ~Application();
	void Run();
private:
	std::unique_ptr<Window> m_Window;
	bool m_IsRunning = true;
};
