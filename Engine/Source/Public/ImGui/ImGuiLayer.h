/**
 * Author: Shon Verch
 * File Name: ImGuiLayer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: Application Layer for handling ImGui.
 */

#pragma once

#include <imgui.h>

#include <Events/Event.h>
#include <Events/ApplicationEvent.h>
#include <Events/MouseEvent.h>
#include <Events/KeyEvent.h>
#include <Layer.h>

/**
 * @class ImGuiLayer ImGuiLayer.h
 * @brief Application Layer for handling ImGui.
 */
class ImGuiLayer final : public Layer
{
public:
	/**
	 * @brief Default constructor.
	 */
	ImGuiLayer();
	
	/**
	 * @brief Default destructor.
	 */
	~ImGuiLayer() override;

	/**
	 * @brief Called when this ImGuiLayer is attached.
	 */
	void OnAttach() override;

	/**
	 * @Brief Called when this ImGuiLayer is detached.
	 */
	void OnDetach() override;

	/**
	 * @brief Update this ImGuiLayer.
	 */
	void OnUpdate() override;

	/**
	 * @brief Handle an Event on this ImGuiLayer.,
	 */
	void OnEvent(Event& event) override;
private:
	/**
	 * @brief Handles the MouseButtonPressedEvent/
	 */
	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event) const;

	/**
	 * @brief Handles the MouseButtonReleasedEvent.
	 */
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event) const;

	/**
	 * @brief Handles the MouseMovedEvent.
	 */
	bool OnMouseMovedEvent(MouseMovedEvent& event) const;

	/**
	 * @brief Handles the MouseScrolledEvent.
	 */
	bool OnMouseScrolledEvent(MouseScrolledEvent& event) const;

	/**
	 * @brief Handles the KeyPressedEvent.
	 */
	bool OnKeyPressedEvent(KeyPressedEvent& event) const;

	/**
	 * @brief Handles the KeyTypedEvent.
	 */
	bool OnKeyTypedEvent(KeyTypedEvent& event) const;

	/**
	 * @brief Handles the KeyReleasedEvent.
	 */
	bool OnKeyReleasedEvent(KeyReleasedEvent& event) const;

	/**
	 * @brief Handles the WindowResizeEvent.
	 */
	bool OnWindowResizedEvent(WindowResizeEvent& event) const;

	/**
	 * @brief The elapsed time, in seconds, between two consecutive frames.
	 */
	float m_DeltaTime = 0;
};