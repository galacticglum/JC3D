/**
 * Author: Shon Verch
 * File Name: Cubemap.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/2019
 * Modified Date: 06/12/2019
 * Description: A cubemap texture.
 */

#pragma once

#include <Renderer/Texture.h>

/**
 * @class Cubemap Cubemap.h
 * @brief A cubemap texture.
 */
class Cubemap : public Texture
{
public:
	/**
	 * @brief Gets the width of this Cubemap.
	 */
	virtual uint32_t GetWidth() const = 0;

	/**
	 * @brief Gets the height of this Cubemap.
	 */
	virtual uint32_t GetHeight() const = 0;

	/**
	 * @brief Gets the format of this Cubemap.
	 */
	virtual TextureFormat GetFormat() const = 0;

	/**
	 * @brief Create a new Cubemap.
	 */
	static Cubemap* Create(const std::string& filepath);
};