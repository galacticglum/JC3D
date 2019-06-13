#include <Platform/OpenGL/OpenGLIndexBuffer.h>
#include <Renderer/Renderer.h>
#include <glad/glad.h>

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t size) 
	: m_Size(size)
{
	ENGINE_RENDER_S({
		glGenBuffers(1, &self->m_IndexBufferId);
	});
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	ENGINE_RENDER_S({
		glDeleteBuffers(1, &self->m_IndexBufferId);
	});
}

void OpenGLIndexBuffer::SetData(void* buffer, unsigned int size, unsigned int offset)
{
	m_Size = size;
	ENGINE_RENDER_S3(buffer, size, offset, {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_IndexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
	});
}

void OpenGLIndexBuffer::Bind() const
{
	ENGINE_RENDER_S({
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_IndexBufferId);
	});
}

void OpenGLIndexBuffer::Unbind() const
{
	ENGINE_RENDER({
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	});
}
