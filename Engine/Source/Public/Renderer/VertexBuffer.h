/**
 * Author: Shon Verch
 * File Name: VertexBuffer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: A vertex buffer.
 */

#pragma once

#include <cstddef>
#include <Renderer/BufferLayout.h>

/**
 * @class VertexBuffer VertexBuffer.h
 * @brief A vertex buffer.
 * @note A vertex buffer object (VBO) allows us to
 *		 upload data to the GPU. The VBO contains vertex
 *	     data and attributes.
 */
class VertexBuffer
{
public:
	/**
	 * @brief Destroy this VertexBuffer.
	 */
	virtual ~VertexBuffer() = default;

	/**
	 * @brief Bind this VertexBuffer.
	 */
	virtual void Bind() const = 0;

	/**
	 * @brief Unbind this VertexBuffer.
	 */
	virtual void Unbind() const = 0;

	/**
	 * @brief Get the BufferLayout of this VertexBuffer.
	 */
	virtual const BufferLayout& GetLayout() const = 0;

	/**
	 * @brief Set the BufferLayout of this VertexBuffer.
	 */
	virtual void SetLayout(const BufferLayout& layout) = 0;

	/**
	 * @brief Create a new VertexBuffer.
	 */
	static VertexBuffer* Create(float* vertices, std::size_t size);
};
