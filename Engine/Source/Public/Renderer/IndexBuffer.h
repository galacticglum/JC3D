/**
 * Author: Shon Verch
 * File Name: IndexBuffer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: An index buffer.
 */

#pragma once

#include <cstdint>

/**
 * @class IndexBuffer IndexBuffer.h
 * @brief An index buffer.
 * @note An index buffer object (IBO) describes the vertex
 *		 indices that should be rendered.
 */
class IndexBuffer
{
public:
	/**
	 * @brief Destroy this IndexBuffer.
	 */
	virtual ~IndexBuffer() = default;

	/**
	 * @brief Bind this IndexBuffer.
	 */
	virtual void Bind() const = 0;

	/**
	 * @brief Unbind this IndexBuffer.
	 */
	virtual void Unbind() const = 0;

	/**
	 * @brief Set the data of this IndexBuffer.
	 */
	virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) = 0;

	/**
	 * @brief Get the number of indices in this IndexBuffer.
	 */
	virtual uint32_t GetCount() const = 0;

	/**
	 * @brief Get the resource handle for this IndexBuffer.
	 */
	virtual uint32_t GetHandle() const = 0;

	/**
	 * @brief Get the size of this IndexBuffer.
	 */
	virtual uint32_t GetSize() const = 0;

	/**
	 * @brief Create a new IndexBuffer.
	 */
	static IndexBuffer* Create(uint32_t size = 0);
};
