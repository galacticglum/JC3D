/**
 * Author: Shon Verch
 * File Name: Material.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/13/2019
 * Modified Date: 06/13/2019
 * Description: A PBR Material.
 */

#pragma once
#include <Math/Vector.h>
#include <Renderer/Texture2D.h>
#include <Renderer/Shader.h>

/**
 * @class Material Material.h
 * @brief A PBR Material.
 */
class Material
{
public:
	/**
	 * @brief The Albedo parameters of this Material.
	 */
	struct AlbedoInput
	{
		/**
		 * @brief The colour of the albedo map.
		 * @note Defaults to "silver", from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
		 */
		Vector3f Color = { 0.972f, 0.96f, 0.915f };

		/**
		 * @brief The albedo map texture.
		 */
		std::unique_ptr<Texture2D> TextureMap;

		/**
		 * @brief A boolean indicating whether the albedo map is sRGB.
		 */
		bool SRGB = true;

		/**
		 * @brief A boolean indicating whether the albedo texture map should be used.
		 */
		bool UseTexture = false;
	} Albedo;

	/**
	 * @brief The Normal parameters of this Material.
	 */
	struct NormalInput
	{
		/**
		 * @brief The normal map texture.
		 */
		std::unique_ptr<Texture2D> TextureMap;

		/**
		 * @brief A boolean indicating whether the normal texture map should be used.
		 */
		bool UseTexture = false;
	} Normal;

	/**
	 * @brief The Metalness parameters of this Material.
	 */
	struct MetalnessInput
	{
		/**
		 * @brief The metallic value.
		 */
		float Value = 1.0f;

		/**
		 * @brief The metallic map texture.
		 */
		std::unique_ptr<Texture2D> TextureMap;

		/**
		 * @brief A boolean indicating whether the metallic texture map should be used.
		 */
		bool UseTexture = false;
	} Metalness;

	/**
	 * @brief The Roughness parameters of this Material.
	 */
	struct RoughnessInput
	{
		/**
		 * @brief The roughness value.
		 */
		float Value = 0.5f;

		/**
		 * @brief The roughness map texture.
		 */
		std::unique_ptr<Texture2D> TextureMap;

		/**
		 * @brief A boolean indicating whether the roughness texture map should be used.
		 */
		bool UseTexture = false;
	} Roughness;

	/**
	 * @brief Bind the material texture maps to their default locations.
	 */
	void BindTextureMaps() const
	{
		if (Albedo.TextureMap)
		{
			Albedo.TextureMap->Bind(1);
		}

		if(Normal.TextureMap)
		{
			Normal.TextureMap->Bind(2);
		}

		if(Metalness.TextureMap)
		{
			Metalness.TextureMap->Bind(3);
		}

		if(Roughness.TextureMap)
		{
			Roughness.TextureMap->Bind(4);
		}
	}
};