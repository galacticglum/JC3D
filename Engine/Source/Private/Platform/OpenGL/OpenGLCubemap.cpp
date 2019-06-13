#include <Platform/OpenGL/OpenGLCubemap.h>
#include <Platform/OpenGL/OpenGLConvertHelper.h>
#include <Renderer/Renderer.h>
#include <Logger.h>

#include <utility>
#include <stb/stb_image.h>
#include <glad/glad.h>

OpenGLCubemap::OpenGLCubemap(std::string filepath) : m_FilePath(std::move(filepath))
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(false);

	m_ImageData = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb);

	m_Width = width;
	m_Height = height;
	m_TextureFormat = TextureFormat::RGB;

	uint32_t faceWidth = m_Width / 4;
	uint32_t faceHeight = m_Height / 3;

	LOG_CATEGORY_ASSERT(faceWidth == faceHeight, "Renderer", "Non-square faces!");

	std::array<unsigned char*, 6> faces;
	for (size_t i = 0; i < faces.size(); i++)
	{
		faces[i] = new unsigned char[faceWidth * faceHeight * 3];
	}

	int faceIndex = 0;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t y = 0; y < faceHeight; y++)
		{
			const size_t yOffset = y + faceHeight;
			for (size_t x = 0; x < faceWidth; x++)
			{
				const size_t xOffset = x + i * faceWidth;
				faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
				faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
				faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
			}
		}

		faceIndex++;
	}

	for (size_t i = 0; i < 3; i++)
	{
		// Skip the middle one
		if (i == 1) continue;

		for (size_t y = 0; y < faceHeight; y++)
		{
			const size_t yOffset = y + i * faceHeight;
			for (size_t x = 0; x < faceWidth; x++)
			{
				const size_t xOffset = x + faceWidth;
				faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
				faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
				faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
			}
		}

		faceIndex++;
	}

	ENGINE_RENDER_S3(faces, faceWidth, faceHeight, {
		glGenTextures(1, &self->m_TextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_TextureId);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameterf(self->m_TextureId, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

		GLenum format = TextureFormatToGL(self->m_TextureFormat);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[0]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[5]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[3]);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_2D, 0);

		for (size_t i = 0; i < faces.size(); i++)
		{
			delete[] faces[i];
		}

		stbi_image_free(self->m_ImageData);
	});
}

OpenGLCubemap::~OpenGLCubemap()
{
	OpenGLCubemap* self = this;
	ENGINE_RENDER_1(self, {
		glDeleteTextures(1, &self->m_TextureId);
	});
}

void OpenGLCubemap::Bind(uint32_t slot) const
{
	ENGINE_RENDER_S1(slot, {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_TextureId);
	});
}

void OpenGLCubemap::Unbind(uint32_t slot) const
{
	ENGINE_RENDER({
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	});
}
