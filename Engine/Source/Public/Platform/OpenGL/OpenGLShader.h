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
	 * @param vertexPath The path to the vertex shader.
	 * @param fragmentPath The path to the fragment shader.
	 * @param geometryPath The path to the geometry shader. This is an optional parameter.
	 *					   If it is null or empty, it is assumed that there is no geometry shader.
	 */
	OpenGLShader(std::string vertexPath, std::string fragmentPath, std::string geometryPath = "");
	
	/**
	 * @brief Destroy this OpenGLShader.
	 */
	~OpenGLShader() override;

	/**
	 * @brief Bind this OpenGLShader.
	 */
	void Bind() const override;

	/**
	 * @brief Unbind this OpenGLShader.
	 */
	void Unbind() const override;

	/**
	 * @brief Gets the path to the vertex shader source file.
	 */
	const std::string& GetVertexPath() const override
	{
		return m_VertexPath;
	}

	/**
	 * @brief Gets the path to the vertex shader source file.
	 */
	const std::string& GetFragmentPath() const override
	{
		return m_FragmentPath;
	}

	/**
	 * @brief Gets the path to the geometry shader source file.
	 * @note This method will return null if there is no geometry shader.
	 */
	const std::string& GetGeometryPath() const override
	{
		return m_GeometryPath;
	}

	/**
	 * @brief Add a uniform with the specified @p uniformName.
	 */
	void AddUniform(const std::string& uniformName) override;

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
	std::string m_VertexPath;
	std::string m_FragmentPath;
	std::string m_GeometryPath;
	uint32_t m_ShaderProgramId;

	std::unordered_map<std::string, GLuint> m_Uniforms;

	/**
	 * @brief Process a shader source file.
	 * @returns A handle to the shader. If the shader is not valid
	 *			or it could not be processed, GL_FALSE is returned instead.
	 */
	static GLuint ProcessShader(const std::string& filePath, GLenum shaderType);
};