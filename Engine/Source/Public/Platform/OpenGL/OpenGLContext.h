/**
 * Author: Shon Verch
 * File Name: OpenGLContext.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: The OpenGL implementation of the RenderContext class.
 */

#pragma once

#include <Rendering/RenderContext.h>

struct GLFWwindow;

/**
 * @class OpenGLContext OpenGLContext.h
 * @brief The OpenGL implementation of the RenderContext class.
 */
class OpenGLContext : public RenderContext
{
public:
	/**
	 * @brief Create a new OpenGLContext given a window handle.
	 */
	explicit OpenGLContext(GLFWwindow* windowHandle);

	/**
	 * @brief Initialize this OpenGLContext.
	 */
	void Initialize() override;

	/**
	 * @brief Swap frame buffers.
	 */
	void SwapBuffers() override;
private:
	GLFWwindow* m_WindowHandle;
};