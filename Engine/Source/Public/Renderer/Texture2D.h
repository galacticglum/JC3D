/**
 * Author: Shon Verch
 * File Name: Texture2D.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/2019
 * Modified Date: 06/11/2019
 * Description: Two-dimensional texture.
 */

#pragma once

#include <cstdint>
#include <Renderer/Texture.h>

/**
 * @class Texture2D Texture2D.h
 * @brief Two-dimensional texture.
 */
class Texture2D : public Texture
{
public:
	/**
	 * @brief Gets the width of this Texture2D.
	 */
	virtual uint32_t GetWidth() const = 0;

	/**
	 * @brief Gets the height of this Texture2D.
	 */
	virtual uint32_t GetHeight() const = 0;

	/**
	 * @brief Gets the format of this Texture2D.
	 */
	virtual TextureFormat GetFormat() const = 0;

	/**
	 * @brief Create a new Texture2D.
	 */
	static Texture2D* Create(TextureFormat format, uint32_t width, uint32_t height);

	/**
	 * @brief Create a new Texture2D from a file.
	 */
	static Texture2D* Create(const std::string& filepath, bool srgb = false);
};