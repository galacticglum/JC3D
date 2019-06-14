/**
 * Author: Shon Verch
 * File Name: EditorLayer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/2019
 * Modified Date: 06/13/2019
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
#include <Renderer/Material.h>
#include <Renderer/SceneObject.h>

#include <imgui.h>

#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Logger.h>

#include <ImguiHelper.h>

/**
 * @brief This layer is responsible for editor demo which includes the PBR renderer.
 * @note This layer simply leverages the API from our engine to render objects in PBR; it is not a renderer itself.
 */
class EditorLayer : public Layer
{
public:
	/**
	 * @brief Initialize a new EditorLayer.
	 */
	EditorLayer() : m_Camera(Matrix4f()), m_SceneType(SceneType::Spheres)
	{
		// Setup our camera with a projection matrix for the size of our window.
		// The camera's projection will be updated later but we initialize the projection
		// matrix here so that our camera is always ready.
		Window& window = Application::Get().GetWindow();
		m_Camera.SetProjectionMatrix(Matrix4f::Perspective(45,  window.GetWidth() / static_cast<float>(window.GetHeight()), 0.1f, 10000.0f));

		m_ModelObject = std::make_unique<SceneObject>();
	}

	/**
	 * @brief Default destructor.
	 */
	virtual ~EditorLayer() = default;

	/**
	 * @brief Called when this layer is attached to the Application context.
	 */
	void OnAttach() override
	{
		// Initialize our shaders
		m_PBRShader.reset(Shader::Create("Content/Shaders/pbr.glsl"));
		m_EnvironmentQuadShader.reset(Shader::Create("Content/Shaders/quad.glsl"));
		m_HDRShader.reset(Shader::Create("Content/Shaders/hdr.glsl"));

		// Load default meshes
		m_ModelObject->SetMesh(new Mesh("Content/Meshes/cerberus.fbx"));
		m_SphereMesh.reset(new Mesh("Content/Models/Sphere.fbx"));
		
		// Editor
		m_CheckerboardTexture.reset(Texture2D::Create("Content/Editor/Checkerboard.tga"));

		// Environment
		m_EnvironmentCubeMap.reset(Cubemap::Create("Content/Textures/Environments/Arches_E_PineTree_Radiance.tga"));
		m_EnvironmentIrradiance.reset(Cubemap::Create("Content/Textures/Environments/Arches_E_PineTree_Irradiance.tga"));

		m_BRDFLUT.reset(Texture2D::Create("Content/Textures/BRDF_LUT.tga"));

		Window& window = Application::Get().GetWindow();
		m_Framebuffer.reset(Framebuffer::Create(window.GetWidth(), window.GetHeight(), FramebufferFormat::RGBA16F));
		m_FinalPresentBuffer.reset(Framebuffer::Create(window.GetWidth(), window.GetHeight(), FramebufferFormat::RGBA8));

		// Create environment quad
		// Used for rendering the environment cubemap/irradiance map
		CreateEnvironmentQuad();

		m_Light.Direction = { -0.5f, -0.5f, 1.0f };
		m_Light.Radiance = { 1.0f, 1.0f, 1.0f };
	}

	/**
	 * @brief Create an environment quad primitive.
	 */
	void CreateEnvironmentQuad()
	{
		// Create Quad
		const float x = -1, y = -1;
		const float width = 2, height = 2;
		struct QuadVertex
		{
			Vector3f Position;
			Vector2f TextureCoordinates;
		};

		// Create the vertex data
		auto* data = new QuadVertex[4];
		data[0].Position = Vector3f(x, y, 0);
		data[0].TextureCoordinates = Vector2f(0, 0);
		data[1].Position = Vector3f(x + width, y, 0);
		data[1].TextureCoordinates = Vector2f(1, 0);
		data[2].Position = Vector3f(x + width, y + height, 0);
		data[2].TextureCoordinates = Vector2f(1, 1);
		data[3].Position = Vector3f(x, y + height, 0);
		data[3].TextureCoordinates = Vector2f(0, 1);

		m_QuadVertexBuffer.reset(VertexBuffer::Create());
		m_QuadVertexBuffer->SetData(data, 4 * sizeof(QuadVertex));

		// Setup simple indices for a quad primitive.
		unsigned* indices = new uint32_t[6]
		{
			0, 1, 2, 2, 3, 0,
		};

		m_QuadIndexBuffer.reset(IndexBuffer::Create());
		m_QuadIndexBuffer->SetData(indices, 6 * sizeof(unsigned int));
	}

	/**
	 * @brief Render the scene.
	 */
	void OnUpdate() override
	{
		m_Camera.Update();
		const Matrix4f viewProjection = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix();

		m_Framebuffer->Bind();
		Renderer::Clear();

		// Upload environment quad data to GPU
		ShaderUniformBufferDeclaration<sizeof(Matrix4f), 1> quadShaderUB;
		quadShaderUB.Push("u_InverseVP", Matrix4f::Inverse(viewProjection));
		m_EnvironmentQuadShader->UploadUniformBuffer(quadShaderUB);

		m_EnvironmentQuadShader->Bind();
		m_EnvironmentIrradiance->Bind(0);

		RenderEnvironmentQuad();
		PushPBRUniformData(viewProjection);

		m_EnvironmentCubeMap->Bind(10);
		m_EnvironmentIrradiance->Bind(11);
		m_BRDFLUT->Bind(15);

		m_PBRShader->Bind();
		m_ModelObject->GetMaterial()->BindTextureMaps();

		UpdateScene();

		m_Framebuffer->Unbind();
		m_FinalPresentBuffer->Bind();
		m_HDRShader->Bind();
		m_HDRShader->SetUniform("u_Exposure", m_Exposure);
		m_Framebuffer->BindTexture();

		RenderEnvironmentQuad();

		m_FinalPresentBuffer->Unbind();
	}

	/**
	 * @brief Render the environment quad.
	 */
	void RenderEnvironmentQuad() const
	{
		m_QuadVertexBuffer->Bind();
		m_QuadIndexBuffer->Bind();
		Renderer::DrawIndexed(m_QuadIndexBuffer->GetCount(), false);
	}

	/**
	 * @brief Update the current scene.
	 */
	void UpdateScene() const
	{
		// Setup the materials for each scene type accordingly.
		// The Spheres scene type is simply a showcase of different
		// material properties on the same mesh: a sphere.
		if (m_SceneType == SceneType::Spheres)
		{
			// Metals
			float roughness = 0.0f;
			float x = -88.0f;
			for (int i = 0; i < 8; i++)
			{
				m_PBRShader->SetUniform("u_ModelMatrix", Matrix4f::Translate(Vector3f(x, 0, 0)));
				m_PBRShader->SetUniform("u_Roughness", roughness);
				m_PBRShader->SetUniform("u_Metalness", 1.0f);
				m_SphereMesh->Render();

				roughness += 0.15f;
				x += 22.0f;
			}

			// Dielectrics
			roughness = 0.0f;
			x = -88.0f;
			for (int i = 0; i < 8; i++)
			{
				m_PBRShader->SetUniform("u_ModelMatrix", Matrix4f::Translate(Vector3f(x, 22, 0)));
				m_PBRShader->SetUniform("u_Roughness", roughness);
				m_PBRShader->SetUniform("u_Metalness", 0.0f);
				m_SphereMesh->Render();

				roughness += 0.15f;
				x += 22.0f;
			}

		}
		// The Model scene type allows the user to select
		// any model...
		else if (m_SceneType == SceneType::Model)
		{
			const std::shared_ptr<Mesh> mesh = m_ModelObject->GetMesh();
			// Make sure that we actually have a mesh to render...
			if (!mesh) return;

			// Update the model matrix of the shader
			m_PBRShader->SetUniform("u_ModelMatrix", m_ModelObject->GetTransformationMatrix());
			m_ModelObject->GetMesh()->Render();
		}
	}

	/**
	 * @brief Push the PBR shader uniform data to the GPU.
	 */
	void PushPBRUniformData(const Matrix4f &viewProjection) const
	{
		// Compute the buffer size as a sum of the individual types.
		// Ideally, I'd like a system where the size is automatically determined but
		// that would require some sort of type interop/RTTI which is a) slow and b)
		// very messy; however, it would be possible to piggy-back on the 
		// ShaderDataType system to create a fixed pipeline of uniform buffer types
		// whose sizes are pre-determined. In this manner, determining the size is as simple
		// as a single iteration over the items.
		ShaderUniformBufferDeclaration<sizeof(Matrix4f) * 2 + sizeof(Vector3f) * 4 + sizeof(float) * 8, 14> uniformBuffer;

		const std::shared_ptr<Material> material = m_ModelObject->GetMaterial();

		uniformBuffer.Push("u_ViewProjectionMatrix", viewProjection);
		uniformBuffer.Push("u_ModelMatrix", Matrix4f());
		uniformBuffer.Push("u_AlbedoColor", material->Albedo.Color);
		uniformBuffer.Push("u_Metalness", material->Metalness.Value);
		uniformBuffer.Push("u_Roughness", material->Roughness.Value);
		uniformBuffer.Push("lights.Direction", m_Light.Direction);
		uniformBuffer.Push("lights.Radiance", m_Light.Radiance * m_LightMultiplier);
		uniformBuffer.Push("u_CameraPosition", m_Camera.GetPosition());
		uniformBuffer.Push("u_RadiancePrefilter", m_RadiancePrefilter ? 1.0f : 0.0f);
		uniformBuffer.Push("u_AlbedoTexToggle", material->Albedo.UseTexture ? 1.0f : 0.0f);
		uniformBuffer.Push("u_NormalTexToggle", material->Normal.UseTexture ? 1.0f : 0.0f);
		uniformBuffer.Push("u_MetalnessTexToggle", material->Metalness.UseTexture ? 1.0f : 0.0f);
		uniformBuffer.Push("u_RoughnessTexToggle", material->Roughness.UseTexture ? 1.0f : 0.0f);
		uniformBuffer.Push("u_EnvMapRotation", m_EnvMapRotation);

		m_PBRShader->UploadUniformBuffer(uniformBuffer);
	}

	/**
	 * @brief Renders a help tooltip with the specified @p name and @p description in ImGui.
	 */
	static void HelpMenuEntry(const char* name, const char* description)
	{
		ImGui::TextDisabled("%s", name);
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
	 * @brief Renders the help menu.
	 */
	static void RenderHelpMenuBar()
	{
		static bool showInfoMenu = false;
		if(showInfoMenu)
		{
			// Open a new ImGui window that displays render stats.
			ImGui::Begin("Renderer Information", &showInfoMenu);

			RenderAPICapabilities& renderApiCapabilities = RendererAPI::GetCapabilities();
			ImGui::Text("Vendor: %s", renderApiCapabilities.Vendor.c_str());
			ImGui::Text("Renderer: %s", renderApiCapabilities.Renderer.c_str());
			ImGui::Text("Version: %s", renderApiCapabilities.Version.c_str());
			ImGui::Separator();
			// Conversion is messy here since we have to convert the fps and deltatime, which are arithmetic types,
			// to a std::string and then convert those into C-style const char* strings.
			ImGui::Text("FPS: %s", std::to_string(Application::Get().GetFPS()).c_str());
			// Convert deltatime to milliseconds
			const float deltaTimeMS = Application::Get().GetDeltaTime() * 1000;
			ImGui::Text("Frame Time: %s seconds", std::to_string(deltaTimeMS).c_str());

			ImGui::End();
		}

		// Help menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Help"))
			{
				// About section
				HelpMenuEntry("About", "A modular and extensible real-time physically based rendering engine library built in modern C++ and OpenGL.\n"
					"At its core, a deferred rendering pipeline powers the engine enabling it to render realistically lit scenes.");

				// What is PBR section
				HelpMenuEntry("What is PBR?", "At the core of the engine sits PBR: a physically-based lighting technique."
					"PBR is based on the theory of microfacets. These are reflective surfaces, that at a microscopic scale,"
					"dictate the appearance of a surface due to various physical properties (e.g. roughness).");

				// Editor explanation section.
				HelpMenuEntry("The Editor", "The Editor allows you to play around with the engine giving two modes: sphere and models."
					"The sphere scene type presents a matrix of spheres showcasing the material properties of PBR."
					"The model scene type allows you to import your own model to play around with the engine.");

				// Camera controls section
				HelpMenuEntry("Camera Controls", "The camera allows you to inspect the currently loaded scene.\n"
					"In order to use the camera, the left ALT key must be held down. To pan, use the middle mouse"
					"button. To rotate, use the left mouse button and then drag. To zoom use the right mouse button"
					"while dragging vertical (up to zoom in, down to zoom out).");

				ImGui::EndMenu();
			}

			ImGui::MenuItem("Info", nullptr, &showInfoMenu);
			ImGui::EndMenuBar();
		}
	}

	/**
	 * @brief Render the material editor UI.
	 */
	void RenderMaterialUI() const
	{
		/**
		 * TODO: Possibly simply this workflow by creating a base material input struct.
		 *		 I think this solution would inevitably create a more modular material workflow
		 *		 but it requires a lot of internal management and comes with significant overhead
		 *		 (i.e. tracking and auto-binding material uniforms and other shader data).
		 *		
		 *		For now, we have to manually define the editor properties for a material.
		 *		
		 *		Documentation is sparse in the following function for it is mostly ImGui commands
		 *		who express their purpose independent of comments; however, I will draw attention to
		 *		some draw particular calls:
		 *			1. PushStyleVar is used extensively to specify the style of a ImGui property
		 *			2. Images are not rendered by passing in image data but rather through their GL
		 *			   handles. Eventually, ImGui should be abstracted to support any graphics API with
		 *			  minimal implementation.
		 */

		const std::shared_ptr<Material> material = m_ModelObject->GetMaterial();

		// Albedo
		if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(material->Albedo.TextureMap ? reinterpret_cast<void*>(material->Albedo.TextureMap->GetHandle()) :
				             reinterpret_cast<void*>(m_CheckerboardTexture->GetHandle()), ImVec2(64, 64));

			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (material->Albedo.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(material->Albedo.TextureMap->GetFilepath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image(reinterpret_cast<void*>(material->Albedo.TextureMap->GetHandle()), ImVec2(384, 384));
					ImGui::EndTooltip();
				}

				if (ImGui::IsItemClicked())
				{
					const std::string filename = Application::Get().OpenFile("");
					if (!filename.empty())
					{
						material->Albedo.TextureMap.reset(Texture2D::Create(filename, material->Albedo.SRGB));
					}
				}
			}

			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Checkbox("Use##AlbedoMap", &material->Albedo.UseTexture);
			if (ImGui::Checkbox("sRGB##AlbedoMap", &material->Albedo.SRGB))
			{
				if (material->Albedo.TextureMap)
				{
					material->Albedo.TextureMap.reset(Texture2D::Create(material->Albedo.TextureMap->GetFilepath(), 
						material->Albedo.SRGB));
				}
			}

			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::ColorEdit3("Color##Albedo", material->Albedo.Color.Data.data(), ImGuiColorEditFlags_NoInputs);
		}
		
		// Normals
		if (ImGui::CollapsingHeader("Normals", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			// Render the image by getting a handle to the material texture
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(material->Normal.TextureMap ? reinterpret_cast<void*>(material->Normal.TextureMap->GetHandle()) :
				             reinterpret_cast<void*>(m_CheckerboardTexture->GetHandle()), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (material->Normal.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(material->Normal.TextureMap->GetFilepath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image(reinterpret_cast<void*>(material->Normal.TextureMap->GetHandle()), ImVec2(384, 384));
					ImGui::EndTooltip();
				}

				if (ImGui::IsItemClicked())
				{
					const std::string filename = Application::Get().OpenFile("");
					if (!filename.empty())
					{
						material->Normal.TextureMap.reset(Texture2D::Create(filename));
					}
				}
			}

			ImGui::SameLine();
			ImGui::Checkbox("Use##NormalMap", &material->Normal.UseTexture);
		}

		// Metalness
		if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			// Render the image by getting a handle to the material texture
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(material->Metalness.TextureMap ? reinterpret_cast<void*>(material->Metalness.TextureMap->GetHandle()) :
				             reinterpret_cast<void*>(m_CheckerboardTexture->GetHandle()), ImVec2(64, 64));

			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (material->Metalness.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(material->Metalness.TextureMap->GetFilepath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image(reinterpret_cast<void*>(material->Metalness.TextureMap->GetHandle()), ImVec2(384, 384));
					ImGui::EndTooltip();
				}

				if (ImGui::IsItemClicked())
				{
					std::string filename = Application::Get().OpenFile("");
					if (!filename.empty())
					{
						material->Metalness.TextureMap.reset(Texture2D::Create(filename));
					}
				}
			}

			ImGui::SameLine();
			ImGui::Checkbox("Use##MetalnessMap", &material->Metalness.UseTexture);
			ImGui::SameLine();
			ImGui::SliderFloat("Value##MetalnessInput", &material->Metalness.Value, 0.0f, 1.0f);
		}

		// Roughness
		if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(material->Roughness.TextureMap ? reinterpret_cast<void*>(
					             material->Roughness.TextureMap->GetHandle()) :
				             reinterpret_cast<void*>(m_CheckerboardTexture->GetHandle()), ImVec2(64, 64));

			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (material->Roughness.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(material->Roughness.TextureMap->GetFilepath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image(reinterpret_cast<void*>(material->Roughness.TextureMap->GetHandle()), ImVec2(384, 384));
					ImGui::EndTooltip();
				}

				if (ImGui::IsItemClicked())
				{
					std::string filename = Application::Get().OpenFile("");
					if (!filename.empty())
					{
						material->Roughness.TextureMap.reset(Texture2D::Create(filename));
					}
				}
			}

			ImGui::SameLine();
			ImGui::Checkbox("Use##RoughnessMap", &material->Roughness.UseTexture);
			ImGui::SameLine();
			ImGui::SliderFloat("Value##RoughnessInput", &material->Roughness.Value, 0.0f, 1.0f);
		}
	}

	/**
	 * @Render the scene object editor.
	 */
	void RenderSceneObjectUI() const
	{
		// Albedo
		if (ImGui::CollapsingHeader("Transform", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGuiProperty("Position", m_ModelObject->Position);
			ImGuiProperty("Rotation", m_ModelObject->Rotation);
			ImGuiProperty("Scale", m_ModelObject->Scale);
		}
	}

	/**
	 * @brief Render the editor UI.
	 */
	void OnImGuiRender() override
	{
		static bool pOpen = true;
		static bool optFullscreenPersistent = true;
		static ImGuiDockNodeFlags flags = ImGuiDockNodeFlags_None;
		const bool optFullscreen = optFullscreenPersistent;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		// Configure imgui for fullscreen
		if (optFullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our 
		// background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (flags & ImGuiDockNodeFlags_PassthruCentralNode)
		{
			windowFlags |= ImGuiWindowFlags_NoBackground;
		}

		// Create the docking workspace
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Editor UI", &pOpen, windowFlags);
		ImGui::PopStyleVar();

		if (optFullscreen)
		{
			ImGui::PopStyleVar(2);
		}

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			const ImGuiID id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(id, ImVec2(0.0f, 0.0f), flags);
		}

		// Editor Panel ------------------------------------------------------------------------------
		ImGui::Begin("Model");
		ImGui::RadioButton("Spheres", reinterpret_cast<int*>(&m_SceneType), static_cast<int>(SceneType::Spheres));
		ImGui::SameLine();
		ImGui::RadioButton("Model", reinterpret_cast<int*>(&m_SceneType), static_cast<int>(SceneType::Model));

		ImGui::Begin("Environment");

		ImGui::Columns(2);
		ImGui::AlignTextToFramePadding();

		// Lighting properties
		ImGuiProperty("Light Direction", m_Light.Direction);
		ImGuiPropertySlider("Light Radiance", m_Light.Radiance, -1, 1, ImguiPropertyFlag::ColorProperty);
		ImGuiProperty("Light Multiplier", m_LightMultiplier, 0.0f, 5.0f);
		ImGuiProperty("Exposure", m_Exposure, 0.0f, 5.0f);
		ImGuiProperty("Radiance Prefiltering", m_RadiancePrefilter);
		ImGuiProperty("Env Map Rotation", m_EnvMapRotation, -360.0f, 360.0f);

		// Mesh selection UI
		ImGui::Columns(1);
		ImGui::End();
		ImGui::Separator();
		{
			ImGui::Text("Mesh");

			std::shared_ptr<Mesh> mesh = m_ModelObject->GetMesh();

			// Get the filepath of the mesh if one is currently loaded.
			const std::string fullpaths = mesh ? mesh->GetFilepath() : "None";

			// We need to get the content path so we can load the mesh.
			const size_t found = fullpaths.find_last_of("/\\");
			const std::string path = found != std::string::npos ? fullpaths.substr(found + 1) : fullpaths;
			ImGui::Text("%s", path.c_str());
			ImGui::SameLine();
			if (ImGui::Button("...##Mesh"))
			{
				const std::string filename = Application::Get().OpenFile("");
				if (!filename.empty())
				{
					m_ModelObject->SetMesh(new Mesh(filename));
				}
			}
		}

		ImGui::Separator();

		// Material input UIs
		// Textures ------------------------------------------------------------------------------
		// We should only draw this if the scene type is model since you can't change the materials
		// of the sphere demo.
		if (m_SceneType == SceneType::Model)
		{
			RenderSceneObjectUI();
			RenderMaterialUI();
		}

		// List all the shaders and give the option to reload them
		// (this is useful option, especially for debugging, since you
		//  can reload shaders without recompiling).
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

		// Render the viewport window.
		// The viewport uses the result of the final present framebuffer computed
		// in the render stage. 
		ImGui::Begin("Viewport");

		// Update the camera projection based on the new viewport size.
		const auto viewportSize = ImGui::GetContentRegionAvail();
		m_Framebuffer->Resize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
		m_FinalPresentBuffer->Resize(static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y));
		m_Camera.SetProjectionMatrix(Matrix4f::Perspective(45, viewportSize.x / viewportSize.y, 0.1f, 10000.0f));

		ImGui::Image(reinterpret_cast<void*>(m_FinalPresentBuffer->GetColorAttachmentHandle()), viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		RenderHelpMenuBar();

		ImGui::End();
	}
private:
	std::unique_ptr<Shader> m_PBRShader;
	std::unique_ptr<Shader> m_EnvironmentQuadShader;
	std::unique_ptr<Shader> m_HDRShader;
	std::unique_ptr<Mesh> m_SphereMesh;
	std::unique_ptr<Texture2D> m_BRDFLUT;

	std::unique_ptr<Framebuffer> m_Framebuffer;
	std::unique_ptr<Framebuffer> m_FinalPresentBuffer;

	std::unique_ptr<VertexBuffer> m_QuadVertexBuffer;
	std::unique_ptr<IndexBuffer> m_QuadIndexBuffer;

	std::unique_ptr<Cubemap> m_EnvironmentCubeMap;
	std::unique_ptr<Cubemap> m_EnvironmentIrradiance;
	
	Camera m_Camera;

	struct Light
	{
		Vector3f Direction;
		Vector3f Radiance;
	} m_Light;

	float m_LightMultiplier = 0.3f;
	float m_Exposure = 1.0f;
	bool m_RadiancePrefilter = false;
	float m_EnvMapRotation = 0.0f;

	/**
	 * @brief The scene type, used for the demo.
	 * @note This is essentially just a binary toggle.
	 */
	enum class SceneType : uint32_t
	{
		Spheres = 0, Model = 1
	};

	SceneType m_SceneType;

	std::unique_ptr<SceneObject> m_ModelObject;
	std::unique_ptr<Texture2D> m_CheckerboardTexture;
};