/**
 * Author: Shon Verch
 * File Name: ImGuiLayer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/10/2019
 * Description: Application Layer for handling ImGui.
 */

#pragma once

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
	 * @brief Called when this ImGuiLayer is detached.
	 */
	void OnDetach() override;

	/**
	 * @brief Render ImGui.
	 */
	void OnImGuiRender() override;

	/**
	 * @brief Begin ImGui rendering.
	 */
	void Begin() const;

	/**
	 * @brief End ImGui rendering.
	 */
	void End() const;
};