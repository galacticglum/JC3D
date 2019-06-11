#include <Renderer/Shader.h>
#include <Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include <Logger.h>

Shader* Shader::Create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::None:
		LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
		return nullptr;
	case RendererAPI::OpenGL:
		return new OpenGLShader(vertexPath, fragmentPath, geometryPath);
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown RendererAPI!");
	return nullptr;
}
