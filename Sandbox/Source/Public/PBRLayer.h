/**
 * Author: Shon Verch
 * File Name: PBRLayer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/2019
 * Modified Date: 06/12/2019
 * Description: Layer responsible for PBR.
 */

#pragma once

#include <Layer.h>
#include <Application.h>
#include <Window.h>

#include <Renderer/Renderer.h>
#include <Renderer/Shader.h>
#include <Renderer/Texture2D.h>
#include <Renderer/Cubemap.h>
#include <Renderer/Framebuffer.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/IndexBuffer.h>
#include <Renderer/Camera.h>
#include <Renderer/Mesh.h>

#include <imgui.h>

#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Logger.h>

static void ImGuiShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

class PBRLayer : public Layer
{
public:
	PBRLayer() : m_Camera(Matrix4f()), m_Scene(Scene::Spheres)
	{
		Window& window = Application::Get().GetWindow();
		m_Camera.SetProjectionMatrix(Matrix4f::Perspective(45, 
			window.GetWidth() / static_cast<float>(window.GetHeight()), 0.1f, 10000.0f));
	}

	virtual ~PBRLayer() = default;

	void OnAttach() override
	{
		m_SimplePBRShader.reset(Shader::Create("Content/Shaders/pbr.glsl"));
		m_QuadShader.reset(Shader::Create("Content/Shaders/quad.glsl"));
		m_HDRShader.reset(Shader::Create("Content/Shaders/hdr.glsl"));
		m_Mesh.reset(new Mesh("Content/Meshes/cerberus.fbx"));
		m_SphereMesh.reset(new Mesh("Content/Models/Sphere.fbx"));

		// Editor
		m_CheckerboardTex.reset(Texture2D::Create("Content/Editor/Checkerboard.tga"));

		// Environment
		m_EnvironmentCubeMap.reset(Cubemap::Create("Content/Textures/Environments/Arches_E_PineTree_Radiance.tga"));
		m_EnvironmentIrradiance.reset(Cubemap::Create("Content/Textures/Environments/Arches_E_PineTree_Irradiance.tga"));

		m_BRDFLUT.reset(Texture2D::Create("Content/Textures/BRDF_LUT.tga"));

		Window& window = Application::Get().GetWindow();
		m_Framebuffer.reset(Framebuffer::Create(window.GetWidth(), window.GetHeight(), FramebufferFormat::RGBA16F));
		m_FinalPresentBuffer.reset(Framebuffer::Create(window.GetWidth(), window.GetHeight(), FramebufferFormat::RGBA8));

		// Create Quad
		const float x = -1, y = -1;
		const float width = 2, height = 2;
		struct QuadVertex
		{
			Vector3f Position;
			Vector2f TexCoord;
		};

		QuadVertex* data = new QuadVertex[4];

		data[0].Position = Vector3f(x, y, 0);
		data[0].TexCoord = Vector2f(0, 0);

		data[1].Position = Vector3f(x + width, y, 0);
		data[1].TexCoord = Vector2f(1, 0);

		data[2].Position = Vector3f(x + width, y + height, 0);
		data[2].TexCoord = Vector2f(1, 1);

		data[3].Position = Vector3f(x, y + height, 0);
		data[3].TexCoord = Vector2f(0, 1);

		m_VertexBuffer.reset(VertexBuffer::Create());
		m_VertexBuffer->SetData(data, 4 * sizeof(QuadVertex));

		uint32_t* indices = new uint32_t[6]{ 0, 1, 2, 2, 3, 0, };
		m_IndexBuffer.reset(IndexBuffer::Create());
		m_IndexBuffer->SetData(indices, 6 * sizeof(unsigned int));

		m_Light.Direction = { -0.5f, -0.5f, 1.0f };
		m_Light.Radiance = { 1.0f, 1.0f, 1.0f };
	}

	void OnDetach() override
	{
	}

	void OnUpdate() override
	{
		// THINGS TO LOOK AT:
		// - BRDF LUT
		// - Cubemap mips and filtering
		// - Tonemapping and proper HDR pipeline

		m_Camera.Update();
		const Matrix4f viewProjection = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix();

		m_Framebuffer->Bind();
		Renderer::Clear();

		ShaderUniformBufferDeclaration<sizeof(Matrix4f), 1> quadShaderUB;
		quadShaderUB.Push("u_InverseVP", Matrix4f::Inverse(viewProjection));
		m_QuadShader->UploadUniformBuffer(quadShaderUB);

		m_QuadShader->Bind();
		m_EnvironmentIrradiance->Bind(0);
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		Renderer::DrawIndexed(m_IndexBuffer->GetCount(), false);

		ShaderUniformBufferDeclaration<sizeof(Matrix4f) * 2 + sizeof(Vector3f) * 4 + sizeof(float) * 8, 14> simplePbrShaderUB;
		simplePbrShaderUB.Push("u_ViewProjectionMatrix", viewProjection);
		simplePbrShaderUB.Push("u_ModelMatrix", Matrix4f());
		simplePbrShaderUB.Push("u_AlbedoColor", m_AlbedoInput.Color);
		simplePbrShaderUB.Push("u_Metalness", m_MetalnessInput.Value);
		simplePbrShaderUB.Push("u_Roughness", m_RoughnessInput.Value);
		simplePbrShaderUB.Push("lights.Direction", m_Light.Direction);
		simplePbrShaderUB.Push("lights.Radiance", m_Light.Radiance * m_LightMultiplier);
		simplePbrShaderUB.Push("u_CameraPosition", m_Camera.GetPosition());
		simplePbrShaderUB.Push("u_RadiancePrefilter", m_RadiancePrefilter ? 1.0f : 0.0f);
		simplePbrShaderUB.Push("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f : 0.0f);
		simplePbrShaderUB.Push("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
		simplePbrShaderUB.Push("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
		simplePbrShaderUB.Push("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
		simplePbrShaderUB.Push("u_EnvMapRotation", m_EnvMapRotation);
		m_SimplePBRShader->UploadUniformBuffer(simplePbrShaderUB);

		m_EnvironmentCubeMap->Bind(10);
		m_EnvironmentIrradiance->Bind(11);
		m_BRDFLUT->Bind(15);

		m_SimplePBRShader->Bind();
		if (m_AlbedoInput.TextureMap)
			m_AlbedoInput.TextureMap->Bind(1);
		if (m_NormalInput.TextureMap)
			m_NormalInput.TextureMap->Bind(2);
		if (m_MetalnessInput.TextureMap)
			m_MetalnessInput.TextureMap->Bind(3);
		if (m_RoughnessInput.TextureMap)
			m_RoughnessInput.TextureMap->Bind(4);

		if (m_Scene == Scene::Spheres)
		{
			// Metals
			float roughness = 0.0f;
			float x = -88.0f;
			for (int i = 0; i < 8; i++)
			{
				m_SimplePBRShader->SetUniform("u_ModelMatrix", Matrix4f::Translate(Vector3f(x, 0, 0)));
				m_SimplePBRShader->SetUniform("u_Roughness", roughness);
				m_SimplePBRShader->SetUniform("u_Metalness", 1.0f);
				m_SphereMesh->Render();

				roughness += 0.15f;
				x += 22.0f;
			}

			// Dielectrics
			roughness = 0.0f;
			x = -88.0f;
			for (int i = 0; i < 8; i++)
			{
				m_SimplePBRShader->SetUniform("u_ModelMatrix", Matrix4f::Translate(Vector3f(x, 22, 0)));
				m_SimplePBRShader->SetUniform("u_Roughness", roughness);
				m_SimplePBRShader->SetUniform("u_Metalness", 0.0f);
				m_SphereMesh->Render();

				roughness += 0.15f;
				x += 22.0f;
			}

		}
		else if (m_Scene == Scene::Model)
		{
			m_Mesh->Render();
		}

		m_Framebuffer->Unbind();

		m_FinalPresentBuffer->Bind();
		m_HDRShader->Bind();
		m_HDRShader->SetUniform("u_Exposure", m_Exposure);
		m_Framebuffer->BindTexture();
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();

		Renderer::DrawIndexed(m_IndexBuffer->GetCount(), false);
		m_FinalPresentBuffer->Unbind();
	}

	enum class PropertyFlag
	{
		None = 0, ColorProperty = 1
	};

	void Property(const std::string& name, bool& value)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		ImGui::Checkbox(id.c_str(), &value);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void Property(const std::string& name, float& value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::None)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		ImGui::SliderFloat(id.c_str(), &value, min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void Property(const std::string& name, Vector3f& value, PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void Property(const std::string& name, Vector3f& value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::None)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if (static_cast<int>(flags) & static_cast<int>(PropertyFlag::ColorProperty))
			ImGui::ColorEdit3(id.c_str(), value.Data.data(), ImGuiColorEditFlags_NoInputs);
		else
			ImGui::SliderFloat3(id.c_str(), value.Data.data(), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void Property(const std::string& name, Vector4f& value, PropertyFlag flags)
	{
		Property(name, value, -1.0f, 1.0f, flags);
	}

	void Property(const std::string& name, Vector4f& value, float min = -1.0f, float max = 1.0f, PropertyFlag flags = PropertyFlag::None)
	{
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		std::string id = "##" + name;
		if (static_cast<int>(flags) & static_cast<int>(PropertyFlag::ColorProperty))
			ImGui::ColorEdit4(id.c_str(), value.Data.data(), ImGuiColorEditFlags_NoInputs);
		else
			ImGui::SliderFloat4(id.c_str(), value.Data.data(), min, max);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	void OnImGuiRender() override
	{
		static bool p_open = true;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		const bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		// Editor Panel ------------------------------------------------------------------------------
		ImGui::Begin("Model");
		ImGui::RadioButton("Spheres", reinterpret_cast<int*>(&m_Scene), static_cast<int>(Scene::Spheres));
		ImGui::SameLine();
		ImGui::RadioButton("Model", reinterpret_cast<int*>(&m_Scene), static_cast<int>(Scene::Model));

		ImGui::Begin("Environment");

		ImGui::Columns(2);
		ImGui::AlignTextToFramePadding();

		Property("Light Direction", m_Light.Direction);
		Property("Light Radiance", m_Light.Radiance, PropertyFlag::ColorProperty);
		Property("Light Multiplier", m_LightMultiplier, 0.0f, 5.0f);
		Property("Exposure", m_Exposure, 0.0f, 5.0f);

		Property("Radiance Prefiltering", m_RadiancePrefilter);
		Property("Env Map Rotation", m_EnvMapRotation, -360.0f, 360.0f);

		ImGui::Columns(1);

		ImGui::End();

		ImGui::Separator();
		{
			ImGui::Text("Mesh");
			std::string fullpath = m_Mesh ? m_Mesh->GetFilepath() : "None";
			size_t found = fullpath.find_last_of("/\\");
			std::string path = found != std::string::npos ? fullpath.substr(found + 1) : fullpath;
			ImGui::Text(path.c_str()); ImGui::SameLine();
			if (ImGui::Button("...##Mesh"))
			{
				std::string filename = Application::Get().OpenFile("");
				if (filename != "")
					m_Mesh.reset(new Mesh(filename));
			}
		}
		ImGui::Separator();

		// Textures ------------------------------------------------------------------------------
		{
			// Albedo
			if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_AlbedoInput.TextureMap ? reinterpret_cast<void*>(m_AlbedoInput.TextureMap->GetHandle()) : 
					reinterpret_cast<void*>(m_CheckerboardTex->GetHandle()), ImVec2(64, 64));

				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_AlbedoInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_AlbedoInput.TextureMap->GetFilepath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image(reinterpret_cast<void*>(m_AlbedoInput.TextureMap->GetHandle()), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						const std::string filename = Application::Get().OpenFile("");
						if (!filename.empty())
						{
							m_AlbedoInput.TextureMap.reset(Texture2D::Create(filename, m_AlbedoInput.SRGB));
						}
					}
				}
				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Checkbox("Use##AlbedoMap", &m_AlbedoInput.UseTexture);
				if (ImGui::Checkbox("sRGB##AlbedoMap", &m_AlbedoInput.SRGB))
				{
					if (m_AlbedoInput.TextureMap)
						m_AlbedoInput.TextureMap.reset(Texture2D::Create(m_AlbedoInput.TextureMap->GetFilepath(), m_AlbedoInput.SRGB));
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::ColorEdit3("Color##Albedo", m_AlbedoInput.Color.Data.data(), ImGuiColorEditFlags_NoInputs);
			}
		}
		{
			// Normals
			if (ImGui::CollapsingHeader("Normals", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_NormalInput.TextureMap ? reinterpret_cast<void*>(m_NormalInput.TextureMap->GetHandle()) : 
					reinterpret_cast<void*>(m_CheckerboardTex->GetHandle()), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_NormalInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_NormalInput.TextureMap->GetFilepath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image(reinterpret_cast<void*>(m_NormalInput.TextureMap->GetHandle()), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						const std::string filename = Application::Get().OpenFile("");
						if (!filename.empty())
						{
							m_NormalInput.TextureMap.reset(Texture2D::Create(filename));
						}
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##NormalMap", &m_NormalInput.UseTexture);
			}
		}
		{
			// Metalness
			if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_MetalnessInput.TextureMap ? reinterpret_cast<void*>(m_MetalnessInput.TextureMap->GetHandle()) : 
					reinterpret_cast<void*>(m_CheckerboardTex->GetHandle()), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_MetalnessInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_MetalnessInput.TextureMap->GetFilepath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image(reinterpret_cast<void*>(m_MetalnessInput.TextureMap->GetHandle()), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Application::Get().OpenFile("");
						if (filename != "")
							m_MetalnessInput.TextureMap.reset(Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##MetalnessMap", &m_MetalnessInput.UseTexture);
				ImGui::SameLine();
				ImGui::SliderFloat("Value##MetalnessInput", &m_MetalnessInput.Value, 0.0f, 1.0f);
			}
		}
		{
			// Roughness
			if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_RoughnessInput.TextureMap ? reinterpret_cast<void*>(m_RoughnessInput.TextureMap->GetHandle()) : 
					reinterpret_cast<void*>(m_CheckerboardTex->GetHandle()), ImVec2(64, 64));

				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_RoughnessInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_RoughnessInput.TextureMap->GetFilepath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image(reinterpret_cast<void*>(m_RoughnessInput.TextureMap->GetHandle()), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename = Application::Get().OpenFile("");
						if (filename != "")
							m_RoughnessInput.TextureMap.reset(Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##RoughnessMap", &m_RoughnessInput.UseTexture);
				ImGui::SameLine();
				ImGui::SliderFloat("Value##RoughnessInput", &m_RoughnessInput.Value, 0.0f, 1.0f);
			}
		}

		ImGui::Separator();

		if (ImGui::TreeNode("Shaders"))
		{
			auto& shaders = Shader::s_AllShaders;
			for (auto& shader : shaders)
			{
				if (ImGui::TreeNode(shader->GetName().c_str()))
				{
					std::string buttonName = "Reload##" + shader->GetName();
					if (ImGui::Button(buttonName.c_str()))
						shader->Reload();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}


		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		auto viewportSize = ImGui::GetContentRegionAvail();
		m_Framebuffer->Resize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
		m_FinalPresentBuffer->Resize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));


		m_Camera.SetProjectionMatrix(Matrix4f::Perspective(45, viewportSize.x / viewportSize.y, 0.1f, 10000.0f));
		ImGui::Image(reinterpret_cast<void*>(m_FinalPresentBuffer->GetColorAttachmentHandle()), viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))
					opt_flags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
					opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))
					opt_flags ^= ImGuiDockNodeFlags_NoResize;
				if (ImGui::MenuItem("Flag: PassthruDockspace", "", (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))
					opt_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (opt_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
					opt_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				ImGui::Separator();
				if (ImGui::MenuItem("Close DockSpace", nullptr, false, p_open != NULL))
					p_open = false;
				ImGui::EndMenu();
			}

			ImGuiShowHelpMarker(
				"You can _always_ dock _any_ window into another by holding the SHIFT key while moving a window. Try it now!" "\n"
				"This demo app has nothing to do with it!" "\n\n"
				"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
				"ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
				"(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
			);

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	virtual void OnEvent(Event& event) override
	{
	}
private:
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Shader> m_PBRShader;
	std::unique_ptr<Shader> m_SimplePBRShader;
	std::unique_ptr<Shader> m_QuadShader;
	std::unique_ptr<Shader> m_HDRShader;
	std::unique_ptr<Mesh> m_Mesh;
	std::unique_ptr<Mesh> m_SphereMesh;
	std::unique_ptr<Texture2D> m_BRDFLUT;

	struct AlbedoInput
	{
		// Silver, from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
		Vector3f Color = { 0.972f, 0.96f, 0.915f };
		std::unique_ptr<Texture2D> TextureMap;
		bool SRGB = true;
		bool UseTexture = false;
	};
	AlbedoInput m_AlbedoInput;

	struct NormalInput
	{
		std::unique_ptr<Texture2D> TextureMap;
		bool UseTexture = false;
	};
	NormalInput m_NormalInput;

	struct MetalnessInput
	{
		float Value = 1.0f;
		std::unique_ptr<Texture2D> TextureMap;
		bool UseTexture = false;
	};
	MetalnessInput m_MetalnessInput;

	struct RoughnessInput
	{
		float Value = 0.5f;
		std::unique_ptr<Texture2D> TextureMap;
		bool UseTexture = false;
	};
	RoughnessInput m_RoughnessInput;

	std::unique_ptr<Framebuffer> m_Framebuffer, m_FinalPresentBuffer;

	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Cubemap> m_EnvironmentCubeMap, m_EnvironmentIrradiance;

	Camera m_Camera;

	struct Light
	{
		Vector3f Direction;
		Vector3f Radiance;
	};

	Light m_Light;
	float m_LightMultiplier = 0.3f;

	// PBR params
	float m_Exposure = 1.0f;
	bool m_RadiancePrefilter = false;
	float m_EnvMapRotation = 0.0f;

	enum class Scene : uint32_t
	{
		Spheres = 0, Model = 1
	};
	Scene m_Scene;

	// Editor resources
	std::unique_ptr<Texture2D> m_CheckerboardTex;
};