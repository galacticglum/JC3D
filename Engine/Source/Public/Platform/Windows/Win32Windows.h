/**
 * Author: Shon Verch
 * File Name: Win32Windows.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: The Win32 implementation of the Window class.
 */

#pragma once

#include <string>
#include <Window.h>

#include <GLFW/glfw3.h>

class Win32Windows : public Window
{
public:
	explicit Win32Windows(const WindowProperties& props);
	virtual ~Win32Windows();

	void OnUpdate() const override;
	uint32_t GetWidth() const override
	{
		return m_Data.Width;
	}

	uint32_t GetHeight() const override
	{
		return m_Data.Height;
	}

	void SetEventCallback(const EventHandler& handler) override
	{
		m_Data.Handler = handler;
	}

	void ToggleVSync(bool enabled) override;
	bool IsVSyncEnabled() const override
	{
		return m_Data.IsVSyncEnabled;
	}

	void* GetNativeWindowContext() const override
	{
		return m_Window;
	}

private:
	virtual void Initialize(const WindowProperties& props);
	virtual void Shutdown();

	GLFWwindow* m_Window;
	struct WindowData
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool IsVSyncEnabled;
		EventHandler Handler;
	} m_Data;
};
