/**
 * Author: Shon Verch
 * File Name: QuadPrimitive.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/13/2019
 * Modified Date: 06/13/2019
 * Description: A quad primitive
 */

#pragma once

#include <Math/Vector.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/IndexBuffer.h>

/**
 * @struct QuadVertex QuadPrimitive.h
 * @brief A vertex of the QuadPrimitive.
 */
struct QuadVertex
{
	/**
	 * @brief The position of this QuadVertex.
	 */
	Vector3f Position;

	/**
	 * @brief The texture coordinate of this QuadVertex.
	 */
	Vector2f TextureCoordinate;
};

/**
 * @class QuadPrimitive QuadPrimitive.h
 * @brief A quad primitive.
 */
class QuadPrimitive
{
public:
	/**
	 * @brief Initialize a new QuadPrimitive.
	 */
	QuadPrimitive(Vector2f position, Vector2f size);

	/**
	 * @brief Get the vertex buffer.
	 */
	const VertexBuffer& GetVertexBuffer() const
	{
		return *m_VertexBuffer;
	}

	/**
	 * @brief Get the index buffer.
	 */
	const IndexBuffer& GetIndexBuffer() const
	{
		return *m_IndexBuffer;
	}
private:
	/**
	 * @brief Vertex buffer for this QuadPrimitive.
	 */
	std::unique_ptr<VertexBuffer> m_VertexBuffer;

	/**
	 * @brief Index buffer for this QuadPrimitive.
	 */
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
};