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
	 * @param indices The data of the OpenGLIndexBuffer.
	 * @param count The number of indices in the data (@p indices).
	 *		  This value is NOT in bytes. It is an element count.
	 */
	explicit OpenGLIndexBuffer(uint32_t* indices, std::size_t count);

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
	 * @brief Gets the number of indices in this IndexBuffer.
	 */
	uint32_t GetCount() const override
	{
		return m_Count;
	}

private:
	uint32_t m_IndexBufferId{};
	uint32_t m_Count{};
};
