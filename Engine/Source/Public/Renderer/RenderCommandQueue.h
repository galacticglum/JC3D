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
	/**
	 * @brief Render command event handler type.
	 */
	typedef void(*RenderCommandHandler)(void*);

	/**
	 * @brief Initialize this RenderCommandQueue.
	 */
	RenderCommandQueue();

	/**
	 * @brief Destroy this RenderCommandQueue.
	 */
	~RenderCommandQueue();

	/**
	 * @brief Allocate a data buffer for the specified render command.
	 */
	void* Allocate(RenderCommandHandler func, unsigned int size);

	/**
	 * @brief Invoke the render commands.
	 */
	void Execute();
private:
	unsigned char* m_CommandBuffer;
	unsigned char* m_CommandBufferPtr;
	unsigned int m_CommandCount = 0;
};