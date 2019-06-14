#include <Renderer/QuadPrimitive.h>

QuadPrimitive::QuadPrimitive(const Vector2f position, const Vector2f size)
{
	QuadVertex* vertexData = new QuadVertex[4];
	vertexData[0].Position = Vector3f(position, 0);
	vertexData[0].TextureCoordinate = Vector2f(0);
	vertexData[1].Position = Vector3f(position.X + size.X, position.Y, 0);
	vertexData[1].TextureCoordinate = Vector2f(1, 0);
	vertexData[2].Position = Vector3f(position + size, 0);
	vertexData[2].TextureCoordinate = Vector2f(1, 1);
	vertexData[3].Position = Vector3f(position.X, position.Y + size.Y, 0);
	vertexData[3].TextureCoordinate = Vector2f(0, 1);

	m_VertexBuffer.reset(VertexBuffer::Create());
	m_VertexBuffer->SetData(vertexData, 4 * sizeof(QuadVertex));

	uint32_t indices[6] = {
		0, 1, 2, 2, 3, 0
	};

	m_IndexBuffer.reset(IndexBuffer::Create());
	m_IndexBuffer->SetData(indices, 6 * sizeof(uint32_t));
}
