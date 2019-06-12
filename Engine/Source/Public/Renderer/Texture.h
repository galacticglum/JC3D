/**
 * Author: Shon Verch
 * File Name: Texture.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/2019
 * Modified Date: 06/11/2019
 * Description: Base class for textures.
 */

#pragma once

#include <cstdint>
#include <string>

/**
 * @enum TextureFormat Texture.h
 * @brief Format modes for textures.
 */
enum class TextureFormat
{
	None = 0,
	RGB,
	RGBA
};

/**
 * @class Texture Texture.h
 * @brief Base class for textures.
 */
class Texture
{
public:
	/**
	 * @brief Destroy this Texture.
	 */
	virtual ~Texture() = default;

	/**
	 * @brief Bind this Texture to the specified @p slot.
	 */
	virtual void Bind(uint32_t slot = 0) const = 0;

	/**
	 * @brief Unbind this Texture from the specified @p slot.
	 */
	virtual void Unbind(uint32_t slot = 0) const = 0;

	/**
	 * @brief Gets the filepath of this Texture.
	 */
	virtual const std::string& GetFilepath() const = 0;

	/**
	 * @brief Gets the texture handle.
	 */
	virtual uint32_t GetHandle() const = 0;
};
