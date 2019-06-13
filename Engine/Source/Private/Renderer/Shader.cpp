#include <Renderer/Shader.h>
#include <Renderer/Renderer.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include <Logger.h>

std::vector<Shader*> Shader::s_AllShaders;

Shader* Shader::Create(const std::string& filepath)
{
	Shader* result = nullptr;
	switch (RendererAPI::Current())
	{
		case RendererAPIType::None:
			LOG_CATEGORY_ASSERT(false, "Renderer", "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPIType::OpenGL:
			result = new OpenGLShader(filepath);
	}

	s_AllShaders.push_back(result);
	return result;
}
