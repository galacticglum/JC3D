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
	explicit OpenGLIndexBuffer(uint32_t size);

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

	/**
	 * @brief Set the data of this OpenGLIndexBuffer.
	 */
	void SetData(void* buffer, uint32_t size, uint32_t offset = 0) override;

	/**
	 * @brief Gets the number of indices in this IndexBuffer.
	 */
	uint32_t GetCount() const override
	{
		return m_Size / sizeof(uint32_t);
	}

	/**
	 * @brief Get the resource handle for this OpenGLIndexBuffer.
	 */
	uint32_t GetHandle() const override
	{
		return m_IndexBufferId;
	}

	/**
	 * @brief Get the size of this OpenGLIndexBuffer.
	 */
	uint32_t GetSize() const override
	{
		return m_Size;
	}
private:
	uint32_t m_IndexBufferId{};
	uint32_t m_Size{};
};
