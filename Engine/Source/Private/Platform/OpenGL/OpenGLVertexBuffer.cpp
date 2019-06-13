#include <Platform/OpenGL/OpenGLVertexBuffer.h>
#include <Renderer/Renderer.h>
#include <glad/glad.h>

OpenGLVertexBuffer::OpenGLVertexBuffer(const uint32_t size)
	: m_Size(size)
{
	ENGINE_RENDER_S({
		glGenBuffers(1, &self->m_VertexBufferId);
	});
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	ENGINE_RENDER_S({
		glDeleteBuffers(1, &self->m_VertexBufferId);
	});
}

void OpenGLVertexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
{
	m_Size = size;
	ENGINE_RENDER_S3(buffer, size, offset, {
		glBindBuffer(GL_ARRAY_BUFFER, self->m_VertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
	});
}

void OpenGLVertexBuffer::Bind() const
{
	ENGINE_RENDER_S({
		glBindBuffer(GL_ARRAY_BUFFER, self->m_VertexBufferId);

		// Enable vertex attrib arrays for positions and texture coordinates.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<const void*>(3 * sizeof(float)));
	});
}

void OpenGLVertexBuffer::Unbind() const
{
	ENGINE_RENDER({
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	})
}
