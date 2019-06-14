#include <Renderer/Renderer.h>

Renderer* Renderer::s_Instance = new Renderer();
RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

void Renderer::Initialize()
{
	ENGINE_RENDER({ RendererAPI::Initialize(); });
}

void Renderer::Clear()
{
	ENGINE_RENDER({ RendererAPI::Clear(0.0f, 0.0f, 0.0f, 1.0f); });
}

void Renderer::Clear(float r, float g, float b, float a)
{
	ENGINE_RENDER_4(r, g, b, a, { RendererAPI::Clear(r, g, b, a); });
}

void Renderer::DrawIndexed(unsigned int count, bool depthTest)
{
	ENGINE_RENDER_2(count, depthTest, {
		RendererAPI::DrawIndexed(count, depthTest);
	});
}

void Renderer::WaitAndRender()
{
	s_Instance->m_CommandQueue.Execute();
}
