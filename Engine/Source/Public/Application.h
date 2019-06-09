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
#include <Events/Event.h>
#include <Window.h>

#include <Events/ApplicationEvent.h>

/**
 * @class Application Application.h
 */
class Application
{
public:
	Application();
	virtual ~Application();
	void Run() const;

	void OnEvent(Event& event);
private:
	bool OnWindowClose(WindowCloseEvent& event);

	std::unique_ptr<Window> m_Window;
	bool m_IsRunning = true;
};
