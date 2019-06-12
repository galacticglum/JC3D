/**
 * Author: Shon Verch
 * File Name: OpenGLRendererAPI.cpp
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: OpenGL implementation of the RendererAPI class.
 */

#include <Renderer/RendererAPI.h>
#include <Logger.h>
#include <glad/glad.h>

static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		Logger::Log("Renderer", LoggerVerbosity::Error, message);
	}
}

void RendererAPI::Initialize()
{
	glDebugMessageCallback(OpenGLLogMessage, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto& caps = GetCapabilities();
	caps.Vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	caps.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	caps.Version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy);
}

void RendererAPI::Shutdown()
{
}

void RendererAPI::Clear(const float r, const float g, const float b, const float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererAPI::SetClearColor(const float r, const float g, const float b, const float a)
{
	glClearColor(r, g, b, a);
}

void RendererAPI::DrawIndexed(const unsigned int count, const bool depthTest)
{
	if (depthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}