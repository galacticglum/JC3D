/**
 * Author: Shon Verch
 * File Name: Renderer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: The engine renderer.
 */

#pragma once

/**
 * @enum RendererAPI Renderer.h
 * @brief An enum of the different types of renderer APIs.
 */
enum class RendererAPI
{
	None = 0,
	OpenGL = 1
};

/**
 * @class Renderer Renderer.h
 * @brief The engine renderer.
 */
class Renderer
{
public:
	/**
	 * @brief Gets the current RendererAPI.
	 */
	static RendererAPI GetAPI()
	{
		return s_RendererAPI;
	}

private:
	static RendererAPI s_RendererAPI;
};