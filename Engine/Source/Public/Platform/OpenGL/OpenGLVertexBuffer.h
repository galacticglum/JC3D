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
	explicit OpenGLVertexBuffer(uint32_t size);

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
	 * @brief Set the data of this OpenGLVertexBuffer.
	 */
	void SetData(void* buffer, uint32_t size, uint32_t offset = 0) override;

	/**
	 * @brief Get the resource handle for this OpenGLVertexBuffer.
	 */
	uint32_t GetHandle() const override
	{
		return m_VertexBufferId;
	}

	/**
	 * @brief Get the size of this OpenGLVertexBuffer.
	 */
	uint32_t GetSize() const override
	{
		return m_Size;
	}
private:
	uint32_t m_VertexBufferId{};
	uint32_t m_Size;
};