/**
 * Author: Shon Verch
 * File Name: OpenGLVertexBuffer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: OpenGL implementation of the VertexBuffer class.
 */

#pragma once

#include <Renderer/VertexBuffer.h>
#include <cstdint>

/**
 * @class OpenGLVertexBuffer OpenGLVertexBuffer.h
 * @brief OpenGL implementation of the VertexBuffer class.
 */
class OpenGLVertexBuffer : public VertexBuffer
{
public:
	/**
	 * @brief Initialize a new OpenGLVertexBuffer.
	 */
	explicit OpenGLVertexBuffer(float* vertices, std::size_t size);

	/**
	 * @brief Destroy this OpenGLVertexBuffer.
	 */
	virtual ~OpenGLVertexBuffer();

	/**
	 * @brief Bind this OpenGLVertexBuffer.
	 */
	void Bind() const override;

	/**
	 * @brief Unbind this OpenGLVertexBuffer.
	 */
	void Unbind() const override;
private:
	uint32_t m_VertexBufferId{};
};
