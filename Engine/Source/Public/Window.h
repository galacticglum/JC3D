/**
 * Author: Shon Verch
 * File Name: Window.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/08/2019
 * Modified Date: 06/08/2019
 * Description: A platform-independent window interface.
 */
#pragma once

#include <Events/Event.h>
#include <string>
#include <functional>
#include <utility>

/**
 * @struct WindowProperties Window.h
 * @brief Generic window properties.
 */
struct WindowProperties
{
	/**
	 * @brief The window title.
	 */
	std::string Title;

	/**
	 * @brief The width of the window.
	 */
	uint32_t Width;

	/**
	 * @brief The height of the window.
	 */
	uint32_t Height;

	/**
	 * @brief Initializes the WindowsProperties with default values
	 *		  (title = "Window Title", width = 1280, height = 720).
	 */
	WindowProperties() : WindowProperties("Window Title", 1280, 720)
	{
	}

	/**
	 * @brief Initializes the WindowsProperties given a title, width, and height.
	 */
	WindowProperties(std::string title, const uint32_t width, const uint32_t height) :
		Title(std::move(title)), Width(width), Height(height)
	{
	}
};

/**
 * @class Window Window.h
 * @brief A platform-independent window interface.
 */
class Window
{
public:
	/**
	 * @brief Window event handler type.
	 */
	using EventHandler = std::function<void(Event&)>;

	/**
	 * @brief Dispose of this Window.
	 */
	virtual ~Window() = default;

	/**
	 * @brief Update this Window.
	 */
	virtual void OnUpdate() const = 0;

	/**
	 * @brief Get the width of this Window, in pixels.
	 */
	virtual uint32_t GetWidth() const = 0;

	/**
	 * @brief Get the height of this Window, in pixels.
	 */
	virtual uint32_t GetHeight() const = 0;

	/**
	 * @brief Set the event handler.
	 */
	virtual void SetEventCallback(const EventHandler& handler) = 0;

	/**
	 * @brief Toggle VSync.
	 */
	virtual void ToggleVSync(bool enabled) = 0;

	/**
	 * @brief Indicates whether VSync is enabled.
	 */
	virtual bool IsVSyncEnabled() const = 0;

	/**
	 * @brief Creates a Window.
	 */
	static Window* Create(const WindowProperties& props = WindowProperties());
};