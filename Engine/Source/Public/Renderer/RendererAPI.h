/**
 * Author: Shon Verch
 * File Name: RendererAPI.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: Platform independent RendererAPI class.
 */

#pragma once
#include <string>

/**
 * @brief The handle type.
 */
using RenderHandleId = uint32_t;

/**
 * @enum RendererAPIType RendererAPI.h
 * @brief The render API type.
 */
enum class RendererAPIType
{
	None,
	OpenGL
};

/**
 * @struct RenderAPICapabilities RendererAPI.h
 * @brief Render API information and settings.
 */
struct RenderAPICapabilities
{
	std::string Vendor;
	std::string Renderer;
	std::string Version;

	int MaxSamples;
	float MaxAnisotropy;
};


/**
 * @class RendererAPI RendererAPI.h
 * @brief Platform independent RendererAPI class.
 */
class RendererAPI
{
public:
	static void Initialize();
	static void Shutdown();

	static void Clear(float r, float g, float b, float a);
	static void SetClearColor(float r, float g, float b, float a);

	static void DrawIndexed(unsigned int count, bool depthTest = true);

	static RenderAPICapabilities& GetCapabilities()
	{
		static RenderAPICapabilities capabilities;
		return capabilities;
	}

	static RendererAPIType Current()
	{
		return s_CurrentRendererAPI;
	}
private:
	static RendererAPIType s_CurrentRendererAPI;
};
