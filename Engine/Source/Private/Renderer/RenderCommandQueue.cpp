#include <Renderer/RenderCommandQueue.h>
#include <cstring>

/**
 * @brief The size of the command buffer.
 * @note 10 megabytes.
 */
#define COMMAND_BUFFER_SIZE (10 * 1024 * 1024)

RenderCommandQueue::RenderCommandQueue()
{
	// Initialize a 10 MB command buffer
	// TODO: Make the command buffer dynamically sized
	m_CommandBuffer = new unsigned char[COMMAND_BUFFER_SIZE];
	m_CommandBufferPtr = m_CommandBuffer;

	memset(m_CommandBuffer, 0, COMMAND_BUFFER_SIZE);
}

RenderCommandQueue::~RenderCommandQueue()
{
	delete[] m_CommandBuffer;
}

void* RenderCommandQueue::Allocate(const RenderCommandHandler func, const unsigned int size)
{
	*reinterpret_cast<RenderCommandHandler*>(m_CommandBufferPtr) = func;
	m_CommandBufferPtr += sizeof(RenderCommandHandler);

	*reinterpret_cast<unsigned int*>(m_CommandBufferPtr) = size;
	m_CommandBufferPtr += sizeof(unsigned int);

	void* memory = m_CommandBufferPtr;
	m_CommandBufferPtr += size;

	m_CommandCount++;
	return memory;
}

void RenderCommandQueue::Execute()
{
	unsigned char* buffer = m_CommandBuffer;
	for (unsigned int i = 0; i < m_CommandCount; i++)
	{
		const RenderCommandHandler function = *reinterpret_cast<RenderCommandHandler*>(buffer);
		buffer += sizeof(RenderCommandHandler);

		const unsigned int size = *reinterpret_cast<unsigned int*>(buffer);
		buffer += sizeof(unsigned int);
		function(buffer);
		buffer += size;
	}

	m_CommandBufferPtr = m_CommandBuffer;
	m_CommandCount = 0;
}