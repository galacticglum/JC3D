#include <Renderer/Texture2D.h>
#include <Platform/OpenGL/OpenGLTexture2D.h>
#include <Renderer/Renderer.h>
#include <Logger.h>

Texture2D* Texture2D::Create(const uint32_t width, const uint32_t height, const TextureParameters parameters,
	const TextureLoadOptions loadOptions)
{
	switch (Renderer::GetAPI())
	{
		case RendererAPI::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLTexture2D(width, height, parameters, loadOptions);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}

Texture2D* Texture2D::Create(const std::string& filepath, TextureParameters parameters, TextureLoadOptions loadOptions)
{
	switch (Renderer::GetAPI())
	{
		case RendererAPI::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLTexture2D(filepath, parameters, loadOptions);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}

Texture2D* Texture2D::Create(const std::string& filepath, TextureLoadOptions loadOptions)
{
	switch (Renderer::GetAPI())
	{
		case RendererAPI::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLTexture2D(filepath, TextureParameters(), loadOptions);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}
