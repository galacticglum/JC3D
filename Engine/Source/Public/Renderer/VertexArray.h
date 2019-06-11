/**
 * Author: Shon Verch
 * File Name: VertexArray.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/19
 * Modified Date: 06/11/19
 * Description: A vertex array containing vertex attribute data.
 */

#pragma once

#include <vector>
#include <memory>

#include <Renderer/VertexBuffer.h>
#include <Renderer/IndexBuffer.h>

/**
 * @class VertexArray VertexArray.h
 * @brief A vertex array.
 * @note The vertex array object (VAO) contains vertex attribute data
 *		 that is then passed to the vertex shader.
 */
class VertexArray
{
public:
	/**
	 * @brief Destroy this VertexArray.
	 */
	virtual ~VertexArray() = default;

	/**
	 * @brief Bind this VertexArray.
	 */
	virtual void Bind() const = 0;

	/**
	 * @brief Unbind this VertexArray.
	 */
	virtual void Unbind() const = 0;

	/**
	 * @brief Add a VertexBuffer to this VertexArray.
	 */
	virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;

	/**
	 * @brief Set the IndexBuffer for this VertexArray.
	 */
	virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

	/**
	 * @brief Get the VertexBuffers of this VertexArray.
	 */
	virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;

	/**
	 * @brief Get the IndexBuffer of this VertexArray.
	 */
	virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

	/**
	 * @brief Create a new VertexArray.
	 */
	static VertexArray* Create();
};
