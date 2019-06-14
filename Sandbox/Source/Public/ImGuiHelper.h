/**
 * Author: Shon Verch
 * File Name: ImGuiHelper.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/13/2019
 * Modified Date: 06/13/2019
 * Description: Helper methods for ImGui rendering.
 */

#pragma once

#include <Math/Vector.h>
#include <string>
#include "imgui.h"

/**
 * @brief Show a helper tooltip.
 */
static void ImGuiShowHelpMarker(const char* description)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(description);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

/**
 * @brief ImGui property flags.
 */
enum class ImguiPropertyFlag
{
	None = 0,
	ColorProperty = 1
};

/**
 * @brief Draw a boolean property (checkbox).
 */
inline void ImGuiProperty(const std::string& name, bool& value)
{
	ImGui::Text(name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	std::string id = "##" + name;
	ImGui::Checkbox(id.c_str(), &value);

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}

/**
 * @brief Draw a numeric slider property.
 */
inline void ImGuiProperty(const std::string& name, float& value, float min = -1.0f, float max = 1.0f, ImguiPropertyFlag flags = ImguiPropertyFlag::None)
{
	ImGui::Text(name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	std::string id = "##" + name;
	ImGui::SliderFloat(id.c_str(), &value, min, max);

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}

/**
 * @Brief Draw a Vector3f property with a minimum and maximum.
 */
inline void ImGuiProperty(const std::string& name, Vector3f& value, float min = -1.0f, float max = 1.0f, ImguiPropertyFlag flags = ImguiPropertyFlag::None)
{
	ImGui::Text(name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	std::string id = "##" + name;
	// If the property has the colour flag on it, draw a color picker as well.
	if (static_cast<int>(flags) & static_cast<int>(ImguiPropertyFlag::ColorProperty))
	{
		ImGui::ColorEdit3(id.c_str(), value.Data.data(), ImGuiColorEditFlags_NoInputs);
	}
	else
	{
		ImGui::SliderFloat3(id.c_str(), value.Data.data(), min, max);
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}

/**
 * @brief Draw a Vector4f property with a minimum and maximum.
 */
inline void ImGuiProperty(const std::string& name, Vector4f& value, float min = -1.0f, float max = 1.0f, ImguiPropertyFlag flags = ImguiPropertyFlag::None)
{
	ImGui::Text(name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	std::string id = "##" + name;
	// Draw a colour picker if this property has the colour flag on it.
	if (static_cast<int>(flags) & static_cast<int>(ImguiPropertyFlag::ColorProperty))
	{
		ImGui::ColorEdit4(id.c_str(), value.Data.data(), ImGuiColorEditFlags_NoInputs);
	}
	else
	{
		ImGui::SliderFloat4(id.c_str(), value.Data.data(), min, max);
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}
