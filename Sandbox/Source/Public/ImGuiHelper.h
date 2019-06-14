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
 * @brief Draw a Vector3f property with a minimum and maximum.
 */
inline void ImGuiProperty(const std::string& name, Vector3f& value, ImguiPropertyFlag flags = ImguiPropertyFlag::None)
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
		ImGui::InputFloat3(id.c_str(), value.Data.data());
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}

/**
 * @brief Draw a Vector3f property with a minimum and maximum.
 */
inline void ImGuiPropertySlider(const std::string& name, Vector3f& value, float min = -1.0f, float max = 1.0f, ImguiPropertyFlag flags = ImguiPropertyFlag::None)
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

/**
 * @brief Draw a Vector4f property with a minimum and maximum.
 */
inline void ImGuiPropertySlider(const std::string& name, Vector4f& value, float min = -1.0f, float max = 1.0f, ImguiPropertyFlag flags = ImguiPropertyFlag::None)
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
/**
 * @brief Draw a Vector4f property.
 */
inline void ImGuiProperty(const std::string& name, Vector4f& value, ImguiPropertyFlag flags = ImguiPropertyFlag::None)
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
		ImGui::InputFloat4(id.c_str(), value.Data.data());
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}

/**
 * @brief Draw a Quaternion property with a minimum and maximum.
 */
inline void ImGuiProperty(const std::string& name, Quaternion& value, bool useDegrees = true)
{
	ImGui::Text(name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	const std::string id = "##" + name;

	// This is a sort of hacky way of rendering quaternion properties.
	// A better way would be to provide a direct access to the quaternion data buffers.
	// Also, we could save on computation by checking whether the angles actually changed.

	// Create a float buffer to store the quaternion euler rotation
	Vector3f eulerAngles = value.ToEulerAngles();
	if (useDegrees)
	{
		// Convert to degrses if the property should be rendered in degrees
		eulerAngles.X = MathFunctions::RadiansToDegree(eulerAngles.X);
		eulerAngles.Y = MathFunctions::RadiansToDegree(eulerAngles.Y);
		eulerAngles.Z = MathFunctions::RadiansToDegree(eulerAngles.Z);
	}

	ImGui::InputFloat3(id.c_str(), eulerAngles.Data.data());

	if(useDegrees)
	{
		// Convert back to radians for the quaternion
		eulerAngles.X = MathFunctions::DegreesToRadians(eulerAngles.X);
		eulerAngles.Y = MathFunctions::DegreesToRadians(eulerAngles.Y);
		eulerAngles.Z = MathFunctions::DegreesToRadians(eulerAngles.Z);
	}

	// Construct a new quaternion based on the euler angles...
	value = Quaternion::FromEulerAngles(eulerAngles);

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}
