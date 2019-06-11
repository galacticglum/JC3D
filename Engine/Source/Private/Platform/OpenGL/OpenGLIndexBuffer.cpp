#include <Platform/OpenGL/OpenGLIndexBuffer.h>
#include <glad/glad.h>

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, const std::size_t size)
{
	glCreateBuffers(1, &m_IndexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_IndexBufferId);
}

void OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
}

void OpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
