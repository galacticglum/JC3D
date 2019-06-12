#include <Renderer/IndexBuffer.h>
#include <Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLIndexBuffer.h>

#include <Logger.h>

IndexBuffer* IndexBuffer::Create(uint32_t* indices, const std::size_t size)
{
	switch (RendererAPI::Current())
	{
		case RendererAPIType::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}
