#include <Renderer/Texture2D.h>
#include <Platform/OpenGL/OpenGLTexture2D.h>
#include <Renderer/Renderer.h>
#include <Logger.h>

Texture2D* Texture2D::Create(const TextureFormat format, const uint32_t width, const uint32_t height)
{
	switch (RendererAPI::Current())
	{
		case RendererAPIType::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLTexture2D(format, width, height);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}

Texture2D* Texture2D::Create(const std::string& filepath, const bool srgb)
{
	switch (RendererAPI::Current())
	{
		case RendererAPIType::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLTexture2D(filepath, srgb);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}
