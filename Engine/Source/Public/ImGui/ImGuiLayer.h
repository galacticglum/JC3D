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
#include <Layer.h>

/**
 * @class ImGuiLayer ImGuiLayer.h
 * @brief Application Layer for handling ImGui.
 */
class ImGuiLayer : public Layer
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
	void OnEvent(const Event& event) override;
private:
	float m_DeltaTime;
};