#include <Renderer/IndexBuffer.h>
#include <Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLIndexBuffer.h>

#include <Logger.h>

IndexBuffer* IndexBuffer::Create(const uint32_t size)
{
	switch (RendererAPI::Current())
	{
		case RendererAPIType::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLIndexBuffer(size);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}
