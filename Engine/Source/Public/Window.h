/**
 * Author: Shon Verch
 * File Name: Window.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/08/2019
 * Modified Date: 06/08/2019
 * Description: Manages the application window.
 */
#pragma once

#include <string>
#include <GLFW/glfw3.h>

/**
 * @class Window Window.H
 * @brief Manages the application window.
 */
class Window
{
public:
	/**
	 * @brief Initialize this Window with the specified @p title and dimensions (@p width and @p height).
	 * @param title The title of the window.
	 * @param width The width of the window, in pixels.
	 * @param height The height of the window, in pixels.
	 */
	Window(std::string title, int width, int height);

	/**
	 * @brief Dispose of this Window.
	 */
	~Window();

	/**
	 * @brief Clear this Window.
	 */
	static void Clear();

	/**
	 * @brief Update this Window.
	 */
	void Update() const;

	/**
	 * @brief Make this Window the current GLFW window context.
	 */
	void MakeContextCurrent() const
	{
		glfwMakeContextCurrent(m_Window);
	}

	/**
	 * @brief Gets whether this window should close.
	 */
	bool IsCloseRequested() const
	{
		return glfwWindowShouldClose(m_Window) == 1;
	}

	/**
	 * @brief Get the width of this Window, in pixels.
	 */
	int GetWidth() const { return m_Width; }

	/**
	 * @brief Get the height of this Window, in pixels.
	 */
	int GetHeight() const { return m_Height; }

	/**
	 * @brief Get the title of this Window.
	 */
	std::string GetTitle() const { return m_Title; }

	/**
	 * @brief Set the title of this Window to the specified @p title.
	 */
	void SetTitle(const std::string& title);
private:
	GLFWwindow* m_Window;
	std::string m_Title;
	int m_Width;
	int m_Height;

	/**
	 * Initialize this Window.
	 */
	bool Initialize();
};