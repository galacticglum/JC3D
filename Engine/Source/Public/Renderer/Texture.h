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
 * @enum TextureWrap Texture.h
 * @brief Texture coordinate wrapping mode.
 */
enum class TextureWrap
{
	None = 0,
	Repeat,
	Clamp,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder
};

/**
 * @enum TextureFilter Texture.h
 * @brief Filtering mode for textures.
 */
enum class TextureFilter
{
	None = 0,
	Linear,
	Nearest
};

/**
 * @enum TextureFormat Texture.h
 * @brief Format modes for textures.
 */
enum class TextureFormat
{
	None = 0,
	RGB,
	RGBA,
	Luminance,
	LuminanceAlpha
};

/**
 * @struct TextureParameters Texture.h
 * @brief Parameters for a Texture.
 */
struct TextureParameters
{
	/**
	 * @brief The format mode for the Texture.
	 */
	TextureFormat Format;

	/**
	 * @brief The filtering mode for the Texture.
	 */
	TextureFilter Filter;

	/**
	 * @brief The wrap mode for the Texture.
	 */
	TextureWrap Wrap;

	/**
	 * @brief Initialize a new TextureParameters.
	 * @note By default, TextureParameters::Format is TextureFormat::RGBA,
	 *		 TextureParameters::Filter is TextureFilter::Linear, and
	 *		 TextureParameters::Wrap is TextureWrap::Clamp.
	 */
	explicit TextureParameters(const TextureFormat format = TextureFormat::RGBA, 
		const TextureFilter filter = TextureFilter::Linear, const TextureWrap wrap = TextureWrap::Clamp)
		: Format(format), Filter(filter), Wrap(wrap)
	{
	}
};

/**
 * @struct TextureLoadOptions Texture.h
 * @brief Texture loading options.
 */
struct TextureLoadOptions
{
	/**
	 * @brief A boolean indicating whether the Texture should be flipped about the x-axis.
	 */
	bool FlipX;

	/**
	 * @brief A boolean indicating whether the Texture should be flipped about the y-axis.
	 */
	bool FlipY;

	/**
	 * @brief Initialize a new TextureLoadOptions.
	 */
	explicit TextureLoadOptions(const bool flipX = false, const bool flipY = false)
		: FlipX(flipX), FlipY(flipY)
	{
	}
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
	 * @brief Gets the name of this Texture.
	 */
	virtual const std::string& GetName() const = 0;

	/**
	 * @brief Gets the filepath of this Texture.
	 */
	virtual const std::string& GetFilepath() const = 0;

	/**
	 * @brief Set the Texture wrap mode.
	 */
	static void SetWrap(const TextureWrap wrapMode)
	{
		s_WrapMode = wrapMode;
	}

	/**
	 * @brief Set the Texture filter mode.
	 */
	static void SetFilter(const TextureFilter filterMode)
	{
		s_FilterMode = filterMode;
	}

	/**
	 * @brief Gets the stride from TextureFormat.
	 */
	static uint32_t GetStride(const TextureFormat textureFormat);
protected:
	static TextureWrap s_WrapMode;
	static TextureFilter s_FilterMode;
};
