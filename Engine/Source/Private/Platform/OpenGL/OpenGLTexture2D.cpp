#include <Platform/OpenGL/OpenGLTexture2D.h>
#include <Platform/OpenGL/OpenGLConvertHelper.h>
#include <Renderer/Renderer.h>
#include <Logger.h>

#include <utility>
#include <glad/glad.h>
#include <stb/stb_image.h>

/**
 * @brief Calculates mipmap count from @p width and @p height.
 */
static int CalculateMipMapCount(const int width, const int height)
{
	int levels = 1;
	while ((width | height) >> levels)
	{
		levels++;
	}

	return levels;
}

OpenGLTexture2D::OpenGLTexture2D(const TextureFormat format, const uint32_t width, const uint32_t height)
	: m_Width(width), m_Height(height), m_TextureFormat(format)
{
	OpenGLTexture2D* self = this;
	ENGINE_RENDER_1(self, {
		glGenTextures(1, &self->m_TextureId);
		glBindTexture(GL_TEXTURE_2D, self->m_TextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameterf(self->m_TextureId, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToGL(self->m_TextureFormat), self->m_Width, 
			self->m_Height, 0, TextureFormatToGL(self->m_TextureFormat), GL_UNSIGNED_BYTE, nullptr);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	});
}

OpenGLTexture2D::OpenGLTexture2D(std::string filepath,  bool srgb)
	: m_FilePath(std::move(filepath)), m_TextureId(0)
{
	int width, height, channels;
	Logger::Log("Renderer", LoggerVerbosity::Info, "Loading texture \"{0}\" (srgb = {1})", filepath, srgb);
	m_ImageData = stbi_load(filepath.c_str(), &width, &height, &channels, srgb ? STBI_rgb : STBI_rgb_alpha);
	m_Width = width;
	m_Height = height;
	m_TextureFormat = TextureFormat::RGBA;

	ENGINE_RENDER_S1(srgb, {
		if (srgb)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &self->m_TextureId);
			const int levels = CalculateMipMapCount(self->m_Width, self->m_Height);
			Logger::Log("Renderer", LoggerVerbosity::Info, "Creating srgb texture width {0} mips", levels);

			glTextureStorage2D(self->m_TextureId, levels, GL_SRGB8, self->m_Width, self->m_Height);
			glTextureParameteri(self->m_TextureId, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
			glTextureParameteri(self->m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureSubImage2D(self->m_TextureId, 0, 0, 0, self->m_Width, self->m_Height,
				GL_RGB, GL_UNSIGNED_BYTE, self->m_ImageData);

			glGenerateTextureMipmap(self->m_TextureId);
		}
		else
		{
			glGenTextures(1, &self->m_TextureId);
			glBindTexture(GL_TEXTURE_2D, self->m_TextureId);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToGL(self->m_TextureFormat), self->m_Width, self->m_Height, 
				0,  srgb ? GL_SRGB8 : TextureFormatToGL(self->m_TextureFormat), GL_UNSIGNED_BYTE, self->m_ImageData);

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		stbi_image_free(self->m_ImageData);
	});
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	OpenGLTexture2D* self = this;
	ENGINE_RENDER_1(self, {
		glDeleteTextures(1, &self->m_TextureId);
	});
}

void OpenGLTexture2D::Bind(const uint32_t slot) const
{
	ENGINE_RENDER_S1(slot, {
			glBindTextureUnit(slot, self->m_TextureId);
	});
}

void OpenGLTexture2D::Unbind(const uint32_t slot) const
{
	ENGINE_RENDER_1(slot, {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	});
}