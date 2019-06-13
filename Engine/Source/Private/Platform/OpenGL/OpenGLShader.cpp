#include <Platform/OpenGL/OpenGLShader.h>

#include <Logger.h>
#include <Renderer/Renderer.h>
#include <Utilities/STLUtilities.h>

#include <string>
#include <fstream>
#include <sstream>
#include <limits>

OpenGLShader::OpenGLShader(const std::string& filepath) : m_Filepath(filepath)
{
	// Find the name of the shader
	size_t found = filepath.find_last_of("/\\");

	// If we couldn't find the name, use the filepath name.
	m_Name = found != std::string::npos ? filepath.substr(found + 1) : filepath;

	Reload();
}

OpenGLShader::~OpenGLShader()
{
	// Delete the shader
	ENGINE_RENDER_S({
		// Delete the shader program if one already exists.
		if (self->m_ShaderProgramId)
		{
			glDeleteShader(self->m_ShaderProgramId);
		}
	});
}


void OpenGLShader::Reload()
{
	ReadFromFile(m_Filepath);
	ENGINE_RENDER_S({
		// Delete the shader program if one already exists.
		if (self->m_ShaderProgramId)
		{
			glDeleteShader(self->m_ShaderProgramId);
		}

		self->Compile();
	});
}

void OpenGLShader::Bind() const
{
	ENGINE_RENDER_S({
		glUseProgram(self->m_ShaderProgramId);
	});
}

void OpenGLShader::Unbind() const
{
	ENGINE_RENDER({
		glUseProgram(0);
	});
}

void OpenGLShader::ReadFromFile(const std::string& filepath)
{
	std::ifstream fileStream(filepath, std::ios::in | std::ios::binary);
	if (fileStream)
	{
		// Determine the size of the shader source
		fileStream.seekg(0, std::ios::end);
		m_ShaderSource.resize(fileStream.tellg());

		// Go back to the beginning so we can read the source code
		fileStream.seekg(0, std::ios_base::beg);
		fileStream.read(&m_ShaderSource[0], m_ShaderSource.size());
		fileStream.close();
	}
	else
	{
		Logger::Log("Renderer", LoggerVerbosity::Warning, "Could not read shader file: \"{0}\"", filepath);
	}
}

void OpenGLShader::Compile()
{
	// We store the shader type as a pair so that
	// we have the GL type and the string type (for logging).
	using ShaderType = std::pair<std::string, GLenum>;
	std::unordered_map<ShaderType, std::string, HashPair> shaders;

	// Parse the shaders for their individual components
	const char* typeToken = "#type";
	const std::size_t typeTokenLength = strlen(typeToken);

	std::size_t position = m_ShaderSource.find(typeToken, 0);
	while (position != std::string::npos)
	{
		const std::size_t eol = m_ShaderSource.find_first_of("\r\n", position);
		LOG_CATEGORY_ASSERT(eol != std::string::npos, "Renderer", "Syntax error while parsing shader file: \"{0}\"", m_Filepath);

		const std::size_t begin = position + typeTokenLength + 1;
		std::string type = m_ShaderSource.substr(begin, eol - begin);

		LOG_CATEGORY_ASSERT(type == "vertex" || type == "fragment", "Renderer", "Encountered invalid shader type while parsing shader file: \"{0}\"", m_Filepath);

		const std::size_t next = m_ShaderSource.find_first_not_of("\r\n", eol);
		position = m_ShaderSource.find(typeToken, next);

		const std::size_t sourceLength = position - (next == std::string::npos ? m_ShaderSource.size() - 1 : next);

		ShaderType shaderType = { type, ShaderTypeFromString(type) };
		shaders[shaderType] = m_ShaderSource.substr(next, sourceLength);
	}

	std::vector<GLuint> shaderIds;
	const GLuint shaderProgramId = glCreateProgram();

	// Create each shader GL resource
	for (auto& pair : shaders)
	{
		const ShaderType type = pair.first;

		const GLuint shaderId = glCreateShader(type.second);
		const GLchar* source = static_cast<const GLchar*>((pair.second.c_str()));

		// Send the shader source to GL
		glShaderSource(shaderId, 1, &source, nullptr);
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

			Logger::Log("Renderer", LoggerVerbosity::Error, "Error compiling {0} shader in file \"{1}\":\n{2}", type.first, m_Filepath, infoLog.data());
			LOG_CATEGORY_ASSERT(false, "Renderer", "Shader compilation failed!");
		}

		shaderIds.push_back(shaderId);
		// Attach the shader to our program
		glAttachShader(shaderProgramId, shaderId);
	}

	// Link our shader program
	glLinkProgram(shaderProgramId);
	GLint isLinked = 0;
	glGetProgramiv(m_ShaderProgramId, GL_LINK_STATUS, static_cast<int*>(&isLinked));

	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_ShaderProgramId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_ShaderProgramId, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program nor the shaders anymore.
		glDeleteProgram(m_ShaderProgramId);
		for (auto id : shaderIds)
		{
			glDeleteShader(id);
		}

		Logger::Log("Renderer", LoggerVerbosity::Error, "Error linking shader in file \"{0}\":\n{1}", m_Filepath, infoLog.data());
		LOG_CATEGORY_ASSERT(false, "Renderer", "Shader program linking failed!");
	}

	// We no longer need the individual shader objects since we have
	// linked them to a shader program. Thus, we detach the shaders to
	// avoid them from leaking.
	for (auto id : shaderIds)
	{
		glDeleteShader(id);
	}

	m_ShaderProgramId = shaderProgramId;

	// Bind default texture unit
	UploadUniformInt("u_Texture", 0);

	// Bind PBR default shader textures
	UploadUniformInt("u_AlbedoTexture", 1);
	UploadUniformInt("u_NormalTexture", 2);
	UploadUniformInt("u_MetalnessTexture", 3);
	UploadUniformInt("u_RoughnessTexture", 4);

	UploadUniformInt("u_EnvRadianceTex", 10);
	UploadUniformInt("u_EnvIrradianceTex", 11);

	UploadUniformInt("u_BRDFLUTTexture", 15);
}

#define UNIQUE_NAME_PASTE2(a, b) a ## b
#define UNIQUE_NAME_PASTE(a, b) UNIQUE_NAME_PASTE2(a, b)

/**
 * @brief Macro to generate the name of a uniform location variable.
 * @note This is used internally to create unique uniform location variable names
 *		 in our macros.
 */
#define UNIQUE_UNIFORM_LOCATION_NAME(name) UNIQUE_NAME_PASTE(uniformLocation_, UNIQUE_NAME_PASTE(name, __LINE__))

/**
 * @brief Macro to verify that a uniform with the specified name exists.
 */
#define VERIFY_UNIFORM(name) 																						\
	glUseProgram(m_ShaderProgramId);																				\
	const GLint UNIQUE_UNIFORM_LOCATION_NAME(name) = glGetUniformLocation(m_ShaderProgramId, name.c_str());			\
	LOG_CATEGORY_ASSERT(UNIQUE_UNIFORM_LOCATION_NAME(name) != -1, "Renderer",										\
		"Error in shader file \"{0}\":\nCould not find location of uniform with name \"{1}\"!",	m_Filepath, name);	\

/**
 * @brief Internal macro to set the value of a uniform.
 */
#define INTERNAL_SET_UNIFORM(func, name, ...) {										\
	VERIFY_UNIFORM(name);															\
	func(UNIQUE_UNIFORM_LOCATION_NAME(name), __VA_ARGS__);							\
}

/**
 * @brief Convert a boolean to an integer.
 */
#define BOOL_TO_INT(v) (v ? 1 : 0)

void OpenGLShader::UploadUniformFloat(const std::string& name, const float value) const
{
	INTERNAL_SET_UNIFORM(glUniform1f, name, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const Vector2f& value) const
{
	INTERNAL_SET_UNIFORM(glUniform2fv, name, 1, &value.Data[0]);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const Vector3f& value) const
{
	INTERNAL_SET_UNIFORM(glUniform3fv, name, 1, &value.Data[0]);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const Vector4f& value) const
{
	INTERNAL_SET_UNIFORM(glUniform4fv, name, 1, &value.Data[0]);
}

void OpenGLShader::UploadUniformInt(const std::string& name, const int value) const
{
	INTERNAL_SET_UNIFORM(glUniform1i, name, value);
}

void OpenGLShader::UploadUniformUint32(const std::string& name, const uint32_t value) const
{
	INTERNAL_SET_UNIFORM(glUniform1ui, name, value);
}

void OpenGLShader::UploadUniformInt2(const std::string& name, const Vector2i& value) const
{
	INTERNAL_SET_UNIFORM(glUniform2iv, name, 1, &value.Data[0]);
}

void OpenGLShader::UploadUniformInt3(const std::string& name, const Vector3i& value) const
{
	INTERNAL_SET_UNIFORM(glUniform3iv, name, 1, &value.Data[0]);
}

void OpenGLShader::UploadUniformInt4(const std::string& name, const Vector4i& value) const
{
	INTERNAL_SET_UNIFORM(glUniform4iv, name, 1, &value.Data[0]);
}

void OpenGLShader::UploadUniformBool(const std::string& name, const bool value) const
{
	UploadUniformInt(name, BOOL_TO_INT(value));
}

void OpenGLShader::UploadUniformBool2(const std::string& name, const Vector2<bool>& value) const
{
	UploadUniformInt2(name, Vector2i(BOOL_TO_INT(value.X), BOOL_TO_INT(value.Y)));
}

void OpenGLShader::UploadUniformBool3(const std::string& name, const Vector3<bool>& value) const
{
	UploadUniformInt3(name, Vector3i(BOOL_TO_INT(value.X), BOOL_TO_INT(value.Y), BOOL_TO_INT(value.Z)));
}

void OpenGLShader::UploadUniformBool4(const std::string& name, const Vector4<bool>& value) const
{
	UploadUniformInt4(name, Vector4i(BOOL_TO_INT(value.X), BOOL_TO_INT(value.Y), BOOL_TO_INT(value.Z), BOOL_TO_INT(value.W)));
}

void OpenGLShader::UploadUniformMatrix4x4(const std::string& name, const Matrix4f& value) const
{
	INTERNAL_SET_UNIFORM(glUniformMatrix4fv, name, 1, GL_FALSE, &value.Data[0][0]);
}

void OpenGLShader::UploadUniformBuffer(const ShaderUniformBufferBase& uniformBuffer) const
{
	for (uint32_t i = 0; i < uniformBuffer.GetUniformCount(); ++i)
	{
		const ShaderUniformDeclaration& declaration = uniformBuffer.GetUniforms()[i];

		const bool isTypeValid = declaration.Type != ShaderDataType::None && declaration.Type != ShaderDataType::Unknown;
		LOG_CATEGORY_ASSERT(i, "Renderer", "Shader uniform declaration with invalid type!");

		const std::string& name = declaration.Name;
		switch(declaration.Type)
		{
			case ShaderDataType::Float:
			{
				float value = *(float*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformFloat(name, value);
				});
			}
			case ShaderDataType::Float2:
			{
				Vector2f value = *(Vector2f*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformFloat2(name, value);
				});
			}
			case ShaderDataType::Float3:
			{
				Vector3f value = *(Vector3f*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformFloat3(name, value);
				});
			}
			case ShaderDataType::Float4:
			{
				Vector4f value = *(Vector4f*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformFloat4(name, value);
				});
			}
			case ShaderDataType::Int:
			{
				int value = *(int*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformInt(name, value);
				});
			}
			case ShaderDataType::Int2:
			{
				Vector2i value = *(Vector2i*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformInt2(name, value);
				});
			}
			case ShaderDataType::Int3:
			{
				Vector3i value = *(Vector3i*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformInt3(name, value);
				});
			}
			case ShaderDataType::Int4:
			{
				Vector4i value = *(Vector4i*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformInt4(name, value);
				});
			}
			case ShaderDataType::Uint32:
			{
				uint32_t value = *(uint32_t*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformUint32(name, value);
				});
			}
			case ShaderDataType::Bool:
			{
				bool value = *(bool*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformBool(name, value);
				});
			}
			case ShaderDataType::Bool2:
			{
				Vector2<bool> value = *(Vector2<bool> *)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformBool2(name, value);
				});
			}
			case ShaderDataType::Bool3:
			{
				Vector3<bool> value = *(Vector3<bool> *)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformBool3(name, value);
				});
			}
			case ShaderDataType::Bool4:
			{
				Vector4<bool> value = *(Vector4<bool> *)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformBool4(name, value);
				});
			}
			case ShaderDataType::Matrix4x4:
			{
				Matrix4f value = *(Matrix4f*)(uniformBuffer.GetBuffer() + declaration.Offset);
				ENGINE_RENDER_S2(name, value, {
					self->UploadUniformMatrix4x4(name, value);
				});
			}
		}
	}
}

void OpenGLShader::SetUniform(const std::string& uniformName, int value) const
{
	ENGINE_RENDER_S2(uniformName, value, {
		self->UploadUniformInt(uniformName, value);
	});
}

void OpenGLShader::SetUniform(const std::string& uniformName, float value) const
{
	ENGINE_RENDER_S2(uniformName, value, {
		self->UploadUniformFloat(uniformName, value);
	});
}

void OpenGLShader::SetUniform(const std::string& uniformName, bool value) const
{
	ENGINE_RENDER_S2(uniformName, value, {
		self->UploadUniformBool(uniformName, value);
	});
}

void OpenGLShader::SetUniform(const std::string& uniformName, const Vector2f& value) const
{
	ENGINE_RENDER_S2(uniformName, value, {
		self->UploadUniformFloat2(uniformName, value);
	});
}

void OpenGLShader::SetUniform(const std::string& uniformName, const Vector3f& value) const
{
	ENGINE_RENDER_S2(uniformName, value, {
		self->UploadUniformFloat3(uniformName, value);
	});
}

void OpenGLShader::SetUniform(const std::string& uniformName, const Vector4f& value) const
{
	ENGINE_RENDER_S2(uniformName, value, {
		self->UploadUniformFloat4(uniformName, value);
	});
}

void OpenGLShader::SetUniform(const std::string& uniformName, const Matrix4f& value) const
{
	ENGINE_RENDER_S2(uniformName, value, {
		self->UploadUniformMatrix4x4(uniformName, value);
	});
}

GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
	{
		return GL_VERTEX_SHADER;
	}

	if (type == "fragment")
	{
		return GL_FRAGMENT_SHADER;
	}

	return GL_NONE;
}
