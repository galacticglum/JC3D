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
	/**
	 * @brief Initialize a new OpenGLCubemap from a file.
	 */
	explicit OpenGLCubemap(std::string filepath);

	/**
	 * @brief Destroy this OpenGLCubemap.
	 */
	~OpenGLCubemap();

	/**
	 * @brief Bind this OpenGLCubemap to the specified texture unit.
	 */
	void Bind(uint32_t slot) const override;

	/**
	 * @bried Unbind this OpenGLCubemap.
	 */
	void Unbind(uint32_t slot) const override;

	/**
	 * @brief Get the width of this OpenGLCubemap.
	 */
	uint32_t GetWidth() const override
	{
		return m_Width;
	}

	/**
	 * @brief Get the height of this OpenGLCubemap.
	 */
	uint32_t GetHeight() const override
	{
		return m_Height;
	}

	/**
	 * @brief Get the filepath of this OpenGLCubemap.
	 */
	const std::string& GetFilepath() const override
	{
		return m_FilePath;
	}

	/**
	 * @brief Get the handle to this OpenGLCubemap.
	 */
	uint32_t GetHandle() const override
	{
		return m_TextureId;
	}

	/**
	 * @brief Get the texture format of this OpenGLCubemap.
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