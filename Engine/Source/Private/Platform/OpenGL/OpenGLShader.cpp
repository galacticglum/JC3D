#include <Platform/OpenGL/OpenGLShader.h>

#include <Logger.h>
#include <System/File.h>

#include <vector>
#include <utility>

OpenGLShader::OpenGLShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath) : 
	m_VertexPath(std::move(vertexPath)), m_FragmentPath(std::move(fragmentPath)), m_GeometryPath(std::move(geometryPath))
{
	// Create a shader program
	m_ShaderProgramId = glCreateProgram();

	// Load our shaders
	const GLuint vertexShaderId = ProcessShader(vertexPath, GL_VERTEX_SHADER);
	const GLuint fragmentShaderId = ProcessShader(fragmentPath, GL_FRAGMENT_SHADER);

	// Make sure that none of our shaders are invalid
	if (vertexShaderId == GL_FALSE || fragmentShaderId == GL_FALSE)
	{
		// We don't need the program nor the shaders anymore.
		glDeleteProgram(m_ShaderProgramId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return;
	}

	// Attach our shaders to our program
	glAttachShader(m_ShaderProgramId, vertexShaderId);
	glAttachShader(m_ShaderProgramId, fragmentShaderId);

	GLuint geometryShaderId = GL_FALSE;
	if (!geometryPath.empty())
	{
		geometryShaderId = ProcessShader(geometryPath, GL_GEOMETRY_SHADER);

		// If the geometry shader is invalid, we will still
		// continue since a geometry shader is optional.
		// We will simply exclude the geometry shader.
		if (geometryShaderId != GL_FALSE)
		{			
			glAttachShader(m_ShaderProgramId, geometryShaderId);
		}
	}

	glLinkProgram(m_ShaderProgramId);
	GLint isLinked = 0;
	glGetProgramiv(m_ShaderProgramId, GL_LINK_STATUS, static_cast<int *>(&isLinked));
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_ShaderProgramId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_ShaderProgramId, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program nor the shaders anymore.
		glDeleteProgram(m_ShaderProgramId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
		if (geometryShaderId != GL_FALSE)
		{
			glDeleteShader(geometryShaderId);
		}

		Logger::Log("Renderer", LoggerVerbosity::Error, "Error linking shader:\n{0}", infoLog.data());
		LOG_CATEGORY_ASSERT(false, "Renderer", "Shader program linking failed!");

		return;
	}

	// We no longer need the individual shader objects since we have
	// linked them to a shader program. Thus, we detach the shaders to
	// avoid them from leaking.
	glDetachShader(m_ShaderProgramId, vertexShaderId);
	glDetachShader(m_ShaderProgramId, fragmentShaderId);
	if (geometryShaderId != GL_FALSE)
	{
		glDetachShader(m_ShaderProgramId, geometryShaderId);
	}
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_ShaderProgramId);
}

void OpenGLShader::Bind() const
{
	glUseProgram(m_ShaderProgramId);
}

void OpenGLShader::Unbind() const
{
	glUseProgram(0);
}

GLuint OpenGLShader::ProcessShader(const std::string& filePath, const GLenum shaderType)
{
	// Read the shader source file
	std::string source = File::Read(filePath);
	// Create an empty shader handle
	const GLuint shaderId = glCreateShader(shaderType);

	// Send the shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* glSource = source.c_str();
	glShaderSource(shaderId, 1, &glSource, nullptr);

	// Compile the shader
	glCompileShader(shaderId);

	GLint isCompiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(shaderId);

		Logger::Log("Renderer", LoggerVerbosity::Error, "Error compiling shader (\"{0}\"):\n{1}", filePath, infoLog.data());
		LOG_CATEGORY_ASSERT(false, "Renderer", "Shader compilation failed!");
		
		return GL_FALSE;
	}

	return shaderId;
}
