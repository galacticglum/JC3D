/**
 * Author: Shon Verch
 * File Name: OpenGLTexture2D.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/2019
 * Modified Date: 06/11/2019
 * Description: OpenGL specific implementation of the Texture2D class.
 */

#pragma once

#include <string>
#include <Renderer/Texture2D.h>

/**
 * @class OpenGLTexture2D OpenGLTexture2D.h
 * @brief OpenGL specific implementation of the Texture2D class.
 */
class OpenGLTexture2D : public Texture2D
{
public:
	/**
	 * brief Initialize a new OpenGLTexture2D. 
	 */
	OpenGLTexture2D(TextureFormat format, uint32_t width, uint32_t height);

	/**
	 * @brief Initialize a new OpenGLTexture2D.
	 */
	explicit OpenGLTexture2D(std::string filepath, bool srgb);

	/**
	 * @brief Destroy this OpenGLTexture2D.
	 */
	~OpenGLTexture2D();

	/**
	 * @brief Bind this OpenGLTexture2D.
	 */
	void Bind(uint32_t slot) const override;

	/**
	 * @brief Unbind this OpenGLTexture2D.
	 */
	void Unbind(uint32_t slot) const override;

	/**
	 * @brief Get the width of this OpenGLTexture2D.
	 */
	uint32_t GetWidth() const override
	{
		return m_Width;
	}

	/**
	 * @brief Get the height of this OpenGLTexture2D.
	 */
	uint32_t GetHeight() const override
	{
		return m_Height;
	}

	/**
	 * @brief Get the filepath of this OpenGLTexture2D.
	 */
	const std::string& GetFilepath() const override
	{
		return m_FilePath;
	}

	/**
	 * @brief Get a handle to this OpenGLTexture2D.
	 */
	uint32_t GetHandle() const override
	{
		return m_TextureId;
	}

	/**
	 * @brief Get the format of this OpenGLTexture2D.
	 */
	TextureFormat GetFormat() const override
	{
		return m_TextureFormat;
	}
private:
	uint32_t m_Width{};
	uint32_t m_Height{};
	TextureFormat m_TextureFormat;
	uint32_t m_TextureId{};

	unsigned char* m_ImageData;
	std::string m_FilePath;
};