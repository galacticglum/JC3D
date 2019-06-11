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
	 * @brief Initialize a new Shader given its vertex and fragment source.
	 */
	Shader(const std::string& vertexSource, const std::string& fragmentSource);

	/**
	 * @brief Destroy this Shader.
	 */
	~Shader();

	/**
	 * @brief Bind this Shader.
	 */
	void Bind() const;

	/**
	 * @brief Unbind this Shader.
	 */
	void Unbind() const;
private:
	uint32_t m_ShaderProgramId;
};