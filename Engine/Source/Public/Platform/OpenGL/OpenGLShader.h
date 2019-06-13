/**
 * Author: Shon Verch
 * File Name: OpenGLShader.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/2019
 * Modified Date: 06/11/2019
 * Description: OpenGL implementation of the Shader class.
 */

#pragma once

#include <string>
#include <Renderer/Shader.h>

#include <glad/glad.h>
#include <unordered_map>

/**
 * @class OpenGLShader OpenGLShader.h
 * @brief OpenGL implementation of the Shader class.
 */
class OpenGLShader : public Shader
{
public:
	/**
	 * @brief Initialize a new OpenGLShader.
	 * @param vertexPath The path to the shader.
	 */
	OpenGLShader(const std::string& filepath);
	
	/**
	 * @brief Destroy this OpenGLShader.
	 */
	~OpenGLShader() override;

	/**
	 * @brief Get the name of this OpenGLShader.
	 */
	const std::string& GetName() const override
	{
		return m_Name;
	}

	/**
	 * @brief Get the filepath of this OpenGLShader.
	 */
	const std::string& GetFilepath() const override
	{
		return m_Filepath;
	}

	/**
	 * @brief Reload this OpenGLShader.
	 */
	void Reload() override;

	/**
	 * @brief Bind this OpenGLShader.
	 */
	void Bind() const override;

	/**
	 * @brief Unbind this OpenGLShader.
	 */
	void Unbind() const override;

	/**
	 * @brief Upload a uniform buffer to this OpenGLShader.
	 */
	void UploadUniformBuffer(const ShaderUniformBufferBase& uniformBuffer) const override;

	/**
	 * @brief Set the value of the integer uniform with the specified @p uniformName.
	 */
	void SetUniform(const std::string& uniformName, int value) const override;

	/**
	 * @brief Set the value of the float uniform with the specified @p uniformName.
	 */
	void SetUniform(const std::string& uniformName, float value) const override;

	/**
	 * @brief Set the value of the boolean uniform with the specified @p uniformName.
	 */
	void SetUniform(const std::string& uniformName, bool value) const override;

	/**
	 * @brief Set the value of the Vector2f uniform with the specified @p uniformName.
	 */
	void SetUniform(const std::string& uniformName, const Vector2f& value) const override;

	/**
	 * @brief Set the value of the Vector3f uniform with the specified @p uniformName.
	 */
	void SetUniform(const std::string& uniformName, const Vector3f& value) const override;

	/**
	 * @brief Set the value of the Vector4f uniform with the specified @p uniformName.
	 */
	void SetUniform(const std::string& uniformName, const Vector4f& value) const override;

	/**
	 * @brief Set the value of the Matrix4f uniform with the specified @p uniformName.
	 */
	void SetUniform(const std::string& uniformName, const Matrix4f& value) const override;
private:
	std::string m_Filepath;
	std::string m_Name;
	std::string m_ShaderSource;

	uint32_t m_ShaderProgramId;

	void ReadFromFile(const std::string& filepath);
	void Compile();

	void UploadUniformFloat(const std::string& name, float value) const;
	void UploadUniformFloat2(const std::string& name, const Vector2f& value) const;
	void UploadUniformFloat3(const std::string& name, const Vector3f& value) const;
	void UploadUniformFloat4(const std::string& name, const Vector4f& value) const;

	void UploadUniformInt(const std::string& name, int value) const;
	void UploadUniformUint32(const std::string& name, uint32_t value) const;
	void UploadUniformInt2(const std::string& name, const Vector2i& value) const;
	void UploadUniformInt3(const std::string& name, const Vector3i& value) const;
	void UploadUniformInt4(const std::string& name, const Vector4i& value) const;

	void UploadUniformBool(const std::string& name, bool value) const;
	void UploadUniformBool2(const std::string& name, const Vector2<bool>& value) const;
	void UploadUniformBool3(const std::string& name, const Vector3<bool>& value) const;
	void UploadUniformBool4(const std::string& name, const Vector4<bool>& value) const;

	void UploadUniformMatrix4x4(const std::string& name, const Matrix4f& value) const;

	static GLenum ShaderTypeFromString(const std::string& type);
};