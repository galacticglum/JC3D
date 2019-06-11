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
	 * @brief Create a new Shader.
	 * @param vertexPath The path to the vertex shader.
	 * @param fragmentPath The path to the fragment shader.
	 * @param geometryPath The path to the geometry shader. This is an optional parameter.
	 *					   If it is null or empty, it is assumed that there is no geometry shader.
	 */
	static Shader* Create(const std::string& vertexPath, const std::string& fragmentPath, 
		const std::string& geometryPath = "");
};