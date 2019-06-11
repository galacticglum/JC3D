#include <Renderer/VertexBuffer.h>
#include <Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLVertexBuffer.h>

#include <Logger.h>

VertexBuffer* VertexBuffer::Create(float* vertices, const std::size_t size)
{
	switch (Renderer::GetAPI())
	{
		case RendererAPI::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}
