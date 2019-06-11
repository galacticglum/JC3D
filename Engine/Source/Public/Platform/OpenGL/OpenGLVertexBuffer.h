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

	/**
	 * @Brief Get the BufferLayout of this OpenGLVertexBuffer.
	 */
	const BufferLayout& GetLayout() const override
	{
		return m_Layout;
	}

	/**
	 * @brief Set the BufferLayout of this OpenGLVertexBuffer.
	 */
	void SetLayout(const BufferLayout& layout) override
	{
		m_Layout = layout;
	}
private:
	uint32_t m_VertexBufferId{};
	BufferLayout m_Layout;
};