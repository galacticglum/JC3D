#include <Renderer/VertexArray.h>
#include <Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLVertexArray.h>

#include <Logger.h>

VertexArray* VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:
		LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
		return nullptr;
	case RendererAPI::OpenGL:
		return new OpenGLVertexArray();
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}
