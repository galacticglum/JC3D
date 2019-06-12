/**
 * Author: Shon Verch
 * File Name: PBRLayer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/2019
 * Modified Date: 06/11/2019
 * Description: The PBR render layer.
 */

#pragma once

#include <vector>
#include <memory>

#include <Math/Vector.h>
#include <Layer.h>

#include <Renderer/Shader.h>
#include <Renderer/Texture2D.h>
#include <Renderer/Camera.h>

struct Light
{
	Vector3f Position;
	Vector4f Colour;
};

/**
 * @clas PBRLayer PBRLayer.h
 * @brief The PBR render layer.
 */
class PBRLayer : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Event& event) override;
private:
	std::unique_ptr<Shader> m_PBRShader;
	std::unique_ptr<Texture2D> m_AlbedoMap;
	std::unique_ptr<Texture2D> m_NormalMap;
	std::unique_ptr<Texture2D> m_MetallicMap;
	std::unique_ptr<Texture2D> m_RoughnessMap;
	std::unique_ptr<Texture2D> m_AmbientOcclusionMap;
	std::unique_ptr<Camera> m_Camera;

	std::vector<Light> m_Lights;
};