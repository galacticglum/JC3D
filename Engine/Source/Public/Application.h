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
#include <Layer.h>
#include <LayerStack.h>
#include <ImGui/ImGuiLayer.h>
#include <glad/glad.h>

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
	Application();

	/**
	 * @brief Destroy this Application.
	 */
	virtual ~Application();

	/**
	 * @brief Run this application.
	 * @note Application::Run contains the main render loop.
	 */
	void Run() const;

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

private:
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
	 * @brief The LayerStack of this Application.
	 */
	LayerStack m_LayerStack;

	/**
	 * @brief The singleton Application instance.
	 */
	static Application* s_Instance;

	GLuint m_VertexArray, m_VertexBuffer, m_IndexBuffer;
};
