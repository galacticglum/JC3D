/**
 * Author: Shon Verch
 * File Name: RenderContext.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: A platform-independent render interface. 
 */

#pragma once

/**
 * @class RenderContext RenderContext.h
 */
class RenderContext
{
public:
	/**
	 * @brief Dispose of this RenderContext.
	 */
	virtual ~RenderContext() = default;
	/**
	 * @brief Initialize this RenderContext.
	 */
	virtual void Initialize() = 0;

	/**
	 * @brief Swap frame buffers.
	 */
	virtual void SwapBuffers() = 0;
};