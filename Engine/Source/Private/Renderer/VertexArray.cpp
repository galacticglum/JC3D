#include <Renderer/VertexArray.h>
#include <Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLVertexArray.h>

#include <Logger.h>

VertexArray* VertexArray::Create()
{
	switch (RendererAPI::Current())
	{
	case RendererAPIType::None:
		LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
		return nullptr;
	case RendererAPIType::OpenGL:
		return new OpenGLVertexArray();
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}
