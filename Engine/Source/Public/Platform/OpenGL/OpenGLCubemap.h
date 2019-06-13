/**
 * Author: Shon Verch
 * File Name: OpenGLCubemap.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/2019
 * Modified Date: 06/12/2019
 * Description: OpenGL implementation of the Cubemap class.
 */

#pragma once

#include <string>
#include <Renderer/Cubemap.h>

 /**
  * @class OpenGLCubemap OpenGLCubemap.h
  * @brief OpenGL implementation of the Cubemap class.
  */
class OpenGLCubemap : public Cubemap
{
public:
	explicit OpenGLCubemap(std::string filepath);
	~OpenGLCubemap();

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