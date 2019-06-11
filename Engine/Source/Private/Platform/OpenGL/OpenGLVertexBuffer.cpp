#include <Platform/OpenGL/OpenGLVertexBuffer.h>
#include <glad/glad.h>

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, const std::size_t size)
{
	glCreateBuffers(1, &m_VertexBufferId);
	OpenGLVertexBuffer::Bind();

	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_VertexBufferId);
}

void OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
}

void OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
