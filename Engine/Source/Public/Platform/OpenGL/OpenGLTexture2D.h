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
	OpenGLTexture2D(uint32_t width, uint32_t height, TextureParameters parameters = TextureParameters(), 
		TextureLoadOptions loadOptions = TextureLoadOptions());

	explicit OpenGLTexture2D(std::string filepath, TextureParameters parameters = TextureParameters(), 
		TextureLoadOptions loadOptions = TextureLoadOptions());

	~OpenGLTexture2D();

	void Bind(uint32_t slot) const override;
	void Unbind(uint32_t slot) const override;

	void SetData(const void* pixels) override;

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

	uint32_t GetHandle() const
	{
		return m_TextureId;
	}

private:
	std::string m_FilePath;
	
	uint32_t m_TextureId;
	uint32_t m_Width{};
	uint32_t m_Height{};

	TextureParameters m_Parameters;
	TextureLoadOptions m_LoadOptions;

	uint32_t Load();

	static uint32_t TextureFormatToGL(TextureFormat format);
	static uint32_t TextureWrapToGL(TextureWrap wrap);
};