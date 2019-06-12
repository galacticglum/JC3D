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
	 * @brief Sets the data of this Texture2D to the specified @p pixels.
	 */
	virtual void SetData(const void* pixels) = 0;

	/**
	 * @brief Sets the data of this Texture2D to the specified @p colour.
	 */
	virtual void SetData(uint32_t colour) = 0;

	/**
	 * @brief Gets the width of this Texture2D.
	 */
	virtual uint32_t GetWidth() const = 0;

	/**
	 * @brief Gets the height of this Texture2D.
	 */
	virtual uint32_t GetHeight() const = 0;

	/**
	 * @brief Create a new Texture2D.
	 */
	static Texture2D* Create(uint32_t width, uint32_t height, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());

	/**
	 * @brief Create a new Texture2D from a file.
	 */
	static Texture2D* Create(const std::string& filepath, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());

	/**
	 * @brief Create a new Texture2D from a file.
	 */
	static Texture2D* Create(const std::string& filepath, TextureLoadOptions loadOptions);

	/**
	 * @brief Create a new Texture2D from a file.
	 */
	static Texture2D* Create(const std::string& name, const std::string& filepath, TextureParameters parameters = TextureParameters(), TextureLoadOptions loadOptions = TextureLoadOptions());

	/**
	 * @brief Create a new Texture2D from a file.
	 */
	static Texture2D* Create(const std::string& name, const std::string& filepath, TextureLoadOptions loadOptions);
};