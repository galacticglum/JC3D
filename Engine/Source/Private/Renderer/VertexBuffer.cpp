#include <Renderer/VertexBuffer.h>
#include <Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLVertexBuffer.h>

#include <Logger.h>

VertexBuffer* VertexBuffer::Create(const uint32_t size)
{
	switch (RendererAPI::Current())
	{
		case RendererAPIType::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLVertexBuffer(size);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}