#include <Renderer/PBRLayer.h>
#include <Application.h>
#include <glad/glad.h>

void PBRLayer::OnAttach()
{
	glEnable(GL_DEPTH_TEST);

	// Load the PBR shader
	m_PBRShader.reset(Shader::Create("Content/Shaders/pbr.vs", "Content/Shader/pbr.fs"));
	// Bind and initialize the PBR shader uniforms
	m_PBRShader->Bind();
	m_PBRShader->SetUniform("albedoMap", 0);
	m_PBRShader->SetUniform("normalMap", 1);
	m_PBRShader->SetUniform("metallicMap", 2);
	m_PBRShader->SetUniform("roughnessMap", 3);
	m_PBRShader->SetUniform("ambientOcclusionMap", 4);

	// Load PBR material textures
	m_AlbedoMap.reset(Texture2D::Create("Content/Textures/pbr/rusted_iron/albedo.png"));
	m_NormalMap.reset(Texture2D::Create("Content/Textures/pbr/rusted_iron/normal.png"));
	m_MetallicMap.reset(Texture2D::Create("Content/Textures/pbr/rusted_iron/metallic.png"));
	m_RoughnessMap.reset(Texture2D::Create("Content/Textures/pbr/rusted_iron/roughness.png"));
	m_AmbientOcclusionMap.reset(Texture2D::Create("Content/Textures/pbr/rusted_iron/ao.png"));

	m_Camera.reset(new Camera(Vector3f(0, 0, 3)));
	m_Lights = {
		{ Vector3f(0, 0, 10), Vector4f(150, 150, 150, 255) }
	};

	Window& window = Application::Get().GetWindow();

	const Matrix4f projection = Matrix4f::Perspective(m_Camera->Zoom, window.GetWidth(), window.GetHeight(), 0.1f, 100.0f);
	m_PBRShader->SetUniform("projection", projection);
}

void PBRLayer::OnDetach()
{
}

void PBRLayer::OnUpdate()
{
}

void PBRLayer::OnEvent(Event& event)
{
}
