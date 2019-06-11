#include <Platform/OpenGL/OpenGLVertexArray.h>
#include <glad/glad.h>

/**
 * @briefs Converts a ShaderDataType to an OpenGL data type.
 */
static GLenum ShaderDataTypeToGLType(const ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		case ShaderDataType::Vector2f: return GL_FLOAT;
		case ShaderDataType::Vector3f: return GL_FLOAT;
		case ShaderDataType::Vector4f: return GL_FLOAT;
		case ShaderDataType::Vector2i: return GL_INT;
		case ShaderDataType::Vector3i: return GL_INT;
		case ShaderDataType::Vector4i: return GL_INT;
		case ShaderDataType::Matrix3f: return GL_FLOAT;
		case ShaderDataType::Matrix4f: return GL_FLOAT;
	}

	LOG_CATEGORY_ASSERT(false, "Renderer", "Unknown ShaderDataType!");
	return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_VertexArrayId);
}

void OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_VertexArrayId);
}

void OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	LOG_CATEGORY_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(),
		"Renderer", "Vertex buffer must have a layout!");

	Bind();
	vertexBuffer->Bind();

	uint32_t index = 0;
	const BufferLayout& layout = vertexBuffer->GetLayout();
	for (const BufferElement& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToGLType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), reinterpret_cast<const void*>(element.Offset));

		index++;
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	Bind();
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}