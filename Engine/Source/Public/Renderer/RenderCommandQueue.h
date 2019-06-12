/**
 * Author: Shon Verch
 * File Name: RenderCommandQueue.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: The queue for rendering commands.
 */

#pragma once

/**
 * @class RenderCommandQueue RenderCommandQueue.h
 * @brief The queue for rendering commands.
 */
class RenderCommandQueue
{
public:
	typedef void(*RenderCommandHandler)(void*);

	RenderCommandQueue();
	~RenderCommandQueue();

	void* Allocate(RenderCommandHandler func, unsigned int size);
	void Execute();
private:
	unsigned char* m_CommandBuffer;
	unsigned char* m_CommandBufferPtr;
	unsigned int m_CommandCount = 0;
};