/**
 * Author: Shon Verch
 * File Name: OpenGLIndexBuffer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: OpenGL implementation of the IndexBuffer class.
 */

#pragma once

#include <Renderer/IndexBuffer.h>
#include <cstdint>

 /**
  * @class OpenGLIndexBuffer OpenGLIndexBuffer.h
  * @brief OpenGL implementation of the IndexBuffer class.
  */
class OpenGLIndexBuffer : public IndexBuffer
{
public:
	/**
	 * @brief Initialize a new OpenGLIndexBuffer.
	 */
	explicit OpenGLIndexBuffer(uint32_t* indices, std::size_t size);

	/**
	 * @brief Destroy this OpenGLIndexBuffer.
	 */
	virtual ~OpenGLIndexBuffer();

	/**
	 * @brief Bind this OpenGLIndexBuffer.
	 */
	void Bind() const override;

	/**
	 * @brief Unbind this OpenGLIndexBuffer.
	 */
	void Unbind() const override;
private:
	uint32_t m_IndexBufferId{};
};
