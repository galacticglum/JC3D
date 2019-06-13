/**
 * Author: Shon Verch
 * File Name: OpenGLConvertHelper.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/2019
 * Modified Date: 06/12/2019
 * Description: OpenGL conversion helper functionality.
 */

#pragma once

#include <Renderer/Texture.h>`
#include <glad/glad.h>

 /**
  * @brief Convert the TextureFormat to OpenGL.
  */
static GLenum TextureFormatToGL(const TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::RGB: return GL_RGB;
		case TextureFormat::RGBA: return GL_RGBA;
	}

	return 0;
}
