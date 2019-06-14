/**
 * Author: Shon Verch
 * File Name: Application.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/12/2019
 * Description: Main application class.
 */

#pragma once

#include <memory>
#include <Events/Event.h>
#include <Window.h>
#include <TimeContext.h>

#include <Events/ApplicationEvent.h>
#include <Layer.h>
#include <LayerStack.h>
#include <ImGui/ImGuiLayer.h>

/**
 * @class Application Application.h
 * @brief Main application class.
 */
class Application
{
public:
	/**
	 * @brief Initialize a new Application.
	 */
	Application(const WindowProperties& windowProperties = WindowProperties());

	/**
	 * @brief Destroy this Application.
	 */
	virtual ~Application();

	/**
	 * @brief Run this application.
	 * @note Application::Run contains the main render loop.
	 */
	void Run();

	/**
	 * @brief Called when this Application is initialized.
	 */
	virtual void OnInitialize()
	{
	}

	/**
	 * @brief Called when this Application updates.
	 */
	virtual void OnUpdate()
	{
	}

	/**
	 * @brief Called when this Application is shutting down.
	 */
	virtual void OnShutdown()
	{
	}

	/**
	 * @brief Render ImGui.
	 */
	void RenderImGui();

	/**
	 * @brief Called when an Event is dispatched.
	 */
	void OnEvent(Event& event);

	/**
	 * @brief Push a Layer onto this Application's LayerStack.
	 */
	void PushLayer(Layer* layer);

	/**
	 * @brief Push an overlay Layer onto this Application's LayerStack.
	 */
	void PushOverlay(Layer* layer);

	/**
	 * @brief Get the Application instance.
	 * @note Application is a singleton class.
	 */
	static Application& Get()
	{
		return *s_Instance;
	}

	/**
	 * @brief Get the Window instance of this Application.
	 */
	Window& GetWindow() const
	{
		return *m_Window;
	}

	/**
	 * @brief Get the time, in seconds, between consecutive frames.
	 */
	float GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	/**
	 * @brief Get the TimeContext instance.
	 */
	TimeContext& GetTimeContext() const
	{
		return *m_TimeContext;
	}

	/**
	 * @brief Open a file dialog with the given @p filter.
	 */
	std::string OpenFile(const std::string& filter) const;
private:
	/**
	 * @brief Handle the WindowResizeEvent.
	 */
	bool OnWindowResize(WindowResizeEvent& event);

	/**
	 * @brief Handle the WindowCloseEvent.
	 */
	bool OnWindowClose(WindowCloseEvent& event);

	/**
	 * @brief The Window instance.
	 */
	std::unique_ptr<Window> m_Window;

	/**
	 * @brief The ImGuiLayer instance.
	 */
	ImGuiLayer* m_ImGuiLayer;

	/**
	 * @brief A boolean indicating whether this Application is running.
	 * @note This field is used by Application::Run to determine whether the render loop
	 *		 should terminate.
	 */
	bool m_IsRunning = true;

	/**
	 * @brief A boolean indicating whether this Application is minimized.
	 */
	bool m_Minimized = false;

	/**
	 * @brief The frames per second that the application is running.
	 */
	int m_FPS;

	/**
	 * @brief The time, in seconds, between consecutive frames.
	 */
	float m_DeltaTime;

	/**
	 * @brief The time context.
	 */
	std::unique_ptr<TimeContext> m_TimeContext;

	/**
	 * @brief The LayerStack of this Application.
	 */
	LayerStack m_LayerStack;

	/**
	 * @brief The singleton Application instance.
	 */
	static Application* s_Instance;
};
