#include <Renderer/Shader.h>

#include <Logger.h>
#include <vector>
#include <glad/glad.h>

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
{
	// Create an empty vertex shader handle
	const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is a NULL character terminated.
	const GLchar* source = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, nullptr);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		Logger::Log("Renderer", LoggerVerbosity::Error, "{0}", infoLog.data());
		LOG_CATEGORY_ASSERT(false, "Renderer", "Vertex shader compilation failed!");

		return;
	}

	// Create an empty fragment shader handle
	const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = static_cast<const GLchar *>(fragmentSource.c_str());
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shaders anymore.
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		Logger::Log("Renderer", LoggerVerbosity::Error, "{0}", infoLog.data());
		LOG_CATEGORY_ASSERT(false, "Renderer", "Fragment shader compilation failed!");

		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_ShaderProgramId = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(m_ShaderProgramId, vertexShader);
	glAttachShader(m_ShaderProgramId, fragmentShader);

	// Link our program
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
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		Logger::Log("Renderer", LoggerVerbosity::Error, "{0}", infoLog.data());
		LOG_CATEGORY_ASSERT(false, "Renderer", "Shader program linking failed!");

		return;
	}

	// We no longer need the individual shader objects since we have
	// linked them to a shader program. Thus, we detach the shaders to
	// avoid them from leaking.
	glDetachShader(m_ShaderProgramId, vertexShader);
	glDetachShader(m_ShaderProgramId, fragmentShader);
}

Shader::~Shader()
{
	// Delete the shader program
	glDeleteProgram(m_ShaderProgramId);
}

void Shader::Bind() const
{
	glUseProgram(m_ShaderProgramId);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}
