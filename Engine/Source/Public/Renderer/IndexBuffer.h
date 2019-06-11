/**
 * Author: Shon Verch
 * File Name: IndexBuffer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: An index buffer.
 */

#pragma once

#include <cstddef>
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
	 * @brief Gets the number of indices in this IndexBuffer.
	 */
	virtual uint32_t GetCount() const = 0;

	/**
	 * @brief Create a new IndexBuffer.
	 */
	static IndexBuffer* Create(uint32_t* indices, std::size_t size);
};
