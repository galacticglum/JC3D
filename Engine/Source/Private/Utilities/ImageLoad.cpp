#include <Utilities/ImageLoad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <Logger.h>

unsigned char* LoadImage(const char* filepath, uint32_t* width, uint32_t* height, uint32_t* components, bool flipY)
{
	int x, y, channels;
	unsigned char* data = stbi_load(filepath, &x, &y, &channels, 0);
	LOG_CATEGORY_ASSERT(data, "Engine", "Could not load image \"{0}\"!", filepath);

	*width = x;
	*height = y;
	*components = channels;

	return data;
}

unsigned char* LoadImage(const std::string& filepath, uint32_t* width, uint32_t* height, uint32_t* components, const bool flipY)
{
	return LoadImage(filepath.c_str(), width, height, components, flipY);
}
