/**
 * Author: Shon Verch
 * File Name: OpenGLVertexArray.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/11/19
 * Modified Date: 06/11/19
 * Description: OpenGL specific implementation of the VertexArray class.
 */

#pragma once

#include <vector>
#include <memory>

#include <Renderer/VertexArray.h>

/**
 * @class OpenGLVertexArray OpenGLVertexArray.h
 * @brief OpenGL specific implementation of the VertexArray class.
 */
class OpenGLVertexArray final : public VertexArray
{
public:
	/**
	 * @brief Initialize a new OpenGLVertexArray.
	 */
	OpenGLVertexArray();

	/**
	 * @brief Bind this OpenGLVertexArray.
	 */
	void Bind() const override;

	/**
	 * @brief Unbind this OpenGLVertexArray.
	 */
	void Unbind() const override;

	/**
	 * @brief Adds a VertexBuffer to this OpenGLVertexArray.
	 */
	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;

	/**
	 * @brief Set the IndexBuffer for this OpenGLVertexArray.
	 */
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

	/**
	 * @brief Gets the VertexBuffers for this OpenGLVertexArray.
	 */
	const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override
	{
		return m_VertexBuffers;
	}

	/**
	 * @brief Gets the IndexBuffer for this OpenGLVertexArray.
	 */
	const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override
	{
		return m_IndexBuffer;
	}

private:
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	uint32_t m_VertexArrayId{};
};