/**
 * Author: Shon Verch
 * File Name: Shader.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: Shader scripts used for rendering.
 */

#pragma once

#include <string>

#include <Math/Vector.h>
#include <Math/Matrix.h>

/**
 * @class Shader Shader.h
 * @brief Shader scripts used for rendering.
 */
class Shader
{
public:
	/**
	 * @brief Destroy this Shader.
	 */
	virtual ~Shader() = default;

	/**
	 * @brief Bind this Shader.
	 */
	virtual void Bind() const = 0;

	/**
	 * @brief Unbind this Shader.
	 */
	virtual void Unbind() const = 0;

	/**
	 * @brief Gets the path to the vertex shader source file.
	 */
	virtual const std::string& GetVertexPath() const = 0;

	/**
	 * @brief Gets the path to the fragment shader source file.
	 */
	virtual const std::string& GetFragmentPath() const = 0;

	/**
	 * @brief Gets the path to the geometry shader source file.
	 * @note This method will return null if there is no geometry shader. 
	 */
	virtual const std::string& GetGeometryPath() const = 0;

	/**
	 * @brief Add a uniform with the specified @p uniformName.
	 */
	virtual void AddUniform(const std::string& uniformName) = 0;

	/**
	 * @brief Set the value of the integer uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, int value) const = 0;

	/**
	 * @brief Set the value of the float uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, float value) const = 0;

	/**
	 * @brief Set the value of the boolean uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, bool value) const = 0;

	/**
	 * @brief Set the value of the Vector2f uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, const Vector2f& value) const = 0;

	/**
	 * @brief Set the value of the Vector3f uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, const Vector3f& value) const = 0;

	/**
	 * @brief Set the value of the Vector4f uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, const Vector4f& value) const = 0;

	/**
	 * @brief Set the value of the Matrix4f uniform with the specified @p uniformName.
	 */
	virtual void SetUniform(const std::string& uniformName, const Matrix4f& value) const = 0;

	/**
	 * @brief Create a new Shader.
	 * @param vertexPath The path to the vertex shader.
	 * @param fragmentPath The path to the fragment shader.
	 * @param geometryPath The path to the geometry shader. This is an optional parameter.
	 *					   If it is null or empty, it is assumed that there is no geometry shader.
	 */
	static Shader* Create(const std::string& vertexPath, const std::string& fragmentPath, 
		const std::string& geometryPath = "");
};