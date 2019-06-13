#include <Renderer/Framebuffer.h>
#include <Platform/OpenGL/OpenGLFramebuffer.h>
#include <Renderer/Renderer.h>
#include <Logger.h>

Framebuffer* Framebuffer::Create(const uint32_t width, const uint32_t height, const FramebufferFormat format)
{
	Framebuffer* result = nullptr;
	switch (RendererAPI::Current())
	{
		case RendererAPIType::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			result = nullptr;
		case RendererAPIType::OpenGL:
			result = new OpenGLFramebuffer(width, height, format);
	}

	FramebufferPool::GetGlobal()->Add(result);
	return result;
}

FramebufferPool* FramebufferPool::s_Instance = new FramebufferPool;

FramebufferPool::FramebufferPool(uint32_t maximumFramebuffers)
{
}

FramebufferPool::~FramebufferPool() = default;

std::weak_ptr<Framebuffer> FramebufferPool::AllocateBuffer()
{
	return std::weak_ptr<Framebuffer>();
}

void FramebufferPool::Add(Framebuffer* framebuffer)
{
	m_Pool.push_back(framebuffer);
}
