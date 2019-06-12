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
	OpenGLTexture2D(TextureFormat format, uint32_t width, uint32_t height);
	explicit OpenGLTexture2D(std::string filepath, bool srgb);

	~OpenGLTexture2D();

	void Bind(uint32_t slot) const override;
	void Unbind(uint32_t slot) const override;

	uint32_t GetWidth() const override
	{
		return m_Width;
	}

	uint32_t GetHeight() const override
	{
		return m_Height;
	}

	const std::string& GetFilepath() const override
	{
		return m_FilePath;
	}

	uint32_t GetHandle() const override
	{
		return m_TextureId;
	}

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