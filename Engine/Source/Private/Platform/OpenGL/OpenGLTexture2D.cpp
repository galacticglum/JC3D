#include <Platform/OpenGL/OpenGLTexture2D.h>
#include <Utilities/ImageLoad.h>
#include <Logger.h>

#include <utility>
#include <glad/glad.h>

OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height, const TextureParameters parameters,
	const TextureLoadOptions loadOptions) : m_FilePath("NULL"), m_Width(width), m_Height(height), 
											m_Parameters(parameters), m_LoadOptions(loadOptions)
{
	m_TextureId = Load();
}

OpenGLTexture2D::OpenGLTexture2D(std::string filepath, const TextureParameters parameters,
	const TextureLoadOptions loadOptions) : m_FilePath(std::move(filepath)), m_Parameters(parameters), 
											m_LoadOptions(loadOptions)
{
	m_TextureId = Load();
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_TextureId);
}

void OpenGLTexture2D::Bind(const uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
}

void OpenGLTexture2D::Unbind(const uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture2D::SetData(const void* pixels)
{
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, TextureFormatToGL(m_Parameters.Format), GL_UNSIGNED_BYTE, pixels);
}

uint32_t OpenGLTexture2D::Load()
{
	unsigned char* pixels = nullptr;
	if (m_FilePath != "NULL")
	{
		uint32_t channels;
		pixels = Image::Load(m_FilePath, &m_Width, &m_Height, &channels, m_LoadOptions.FlipY);
		if (channels != 1 && channels != 3 && channels != 4)
		{
			Logger::Log("Renderer", LoggerVerbosity::Error, "Unsupported image bit-depth! (channels: {0}, filepath: {1})!", channels, m_FilePath);
		}

		if (channels == 1)
		{
			m_Parameters.Format = TextureFormat::Luminance;
		}
		else if (channels == 3)
		{
			m_Parameters.Format = TextureFormat::RGB;
		}
		else if (channels == 4)
		{
			m_Parameters.Format = TextureFormat::RGBA;
		}
	}

	uint32_t textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Setup the texture parameters based on our texture parameter info.
	const GLenum minFilter = m_Parameters.Filter == TextureFilter::Linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST;
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

	const GLenum magFilter = m_Parameters.Filter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST;
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	const GLenum wrap = TextureWrapToGL(s_WrapMode);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	const GLenum format = TextureFormatToGL(m_Parameters.Format);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, pixels ? pixels : nullptr);

	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture since we are done.
	glBindTexture(GL_TEXTURE_2D, 0);

	// Free the pixels now that we are done with them.
	// Since we have our texture object, we don't need the raw data.
	delete pixels;


	return textureId;
}

uint32_t OpenGLTexture2D::TextureFormatToGL(const TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::RGB: return GL_RGB;
		case TextureFormat::RGBA: return GL_RGBA;
		case TextureFormat::Luminance: return GL_RED;
		case TextureFormat::LuminanceAlpha: return GL_RG;
	}

	return 0;
}

uint32_t OpenGLTexture2D::TextureWrapToGL(const TextureWrap wrap)
{
	switch (wrap)
	{
		case TextureWrap::Repeat: return GL_REPEAT;
		case TextureWrap::Clamp: return GL_CLAMP_TO_EDGE;
		case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
		case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
		case TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
	}

	return 0;
}
