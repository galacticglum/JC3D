#include <Renderer/Cubemap.h>
#include <Platform/OpenGL/OpenGLCubemap.h>
#include <Renderer/Renderer.h>
#include <Logger.h>

Cubemap* Cubemap::Create(const std::string& filepath)
{
	switch (RendererAPI::Current())
	{
	case RendererAPIType::None:
		LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
		return nullptr;
	case RendererAPIType::OpenGL:
		return new OpenGLCubemap(filepath);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}