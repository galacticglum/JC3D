/**
 * Author: Shon Verch
 * File Name: Win32Window.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: The Win32 implementation of the Window class.
 */

#pragma once

#include <string>
#include <Window.h>

#include <GLFW/glfw3.h>
#include <Renderer/RenderContext.h>

/**
 * @class Win32Window Win32Window.h
 * @brief The Win32 implementation of the Window class.
 * @note This implementation uses GLFW in the backend; there
 *		 is not reason to use the WIN32 API since GLFW is 
 *		 cross-platform (i.e. Windows, Mac, Linux) and cleaner. 
 */
class Win32Window final : public Window
{
public:
	/**
	 * @brief Initialize a new Win32Window given the WindowProperties @p props.
	 */
	explicit Win32Window(const WindowProperties& props);

	/**
	 * @brief Destroy this Win32Window.
	 */
	virtual ~Win32Window();

	/**
	 * @brief Update this Win32Window.
	 */
	void OnUpdate() const override;

	/**
	 * @brief Get the width of this Win32Window.
	 */
	uint32_t GetWidth() const override
	{
		return m_Data.Width;
	}

	/**
	 * @brief Get the height of this Win32Window.
	 */
	uint32_t GetHeight() const override
	{
		return m_Data.Height;
	}

	/**
	 * @brief Sets the event callback to the specified @p handler.
	 */
	void SetEventCallback(const EventHandler& handler) override
	{
		m_Data.Handler = handler;
	}

	/**
	 * @brief Toggles VSync.
	 * @param enabled A boolean indicating whether VSync should be turned on or off.
	 */
	void ToggleVSync(bool enabled) override;

	/**
	 * @brief Gets whether VSync is enabled.
	 */
	bool IsVSyncEnabled() const override
	{
		return m_Data.IsVSyncEnabled;
	}

	/**
	 * @brief Gets the native window context.
	 * @note The native window context refers to the
	 *		 backend window implementation. In this case,
	 *		 the window context is a GLFWwindow.
	 */
	void* GetNativeWindowContext() const override
	{
		return m_Window;
	}

private:
	/**
	 * @brief Initialize this Win32Window.
	 * @param props The properties of this Win32Window.
	 */
	void Initialize(const WindowProperties& props);

	/**
	 * @brief Initialize the events for this Win32Window.
	 */
	void InitializeEvents() const;

	/**
	 * @brief Shutdown this Win32Window.
	 */
	void Shutdown() const;

	/**
	 * @brief The GLFW window.
	 * @note The native window context.
	 */
	GLFWwindow* m_Window;

	/**
	 * @brief The RenderContext instance for this Window.
	 */
	RenderContext* m_RenderContext{};

	/**
	 * @brief The data of this window.
	 * @note This data is separated into its own distinct
	 *	     data structure so that we can simply pass WindowData
	 *		 to GLFW rather than the whole window.
	 */
	struct WindowData
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool IsVSyncEnabled;
		EventHandler Handler;
	} m_Data;
};
