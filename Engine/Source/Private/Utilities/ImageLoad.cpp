#include <Utilities/ImageLoad.h>
#include <Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

unsigned char* Image::Load(const char* filepath, uint32_t* width, uint32_t* height, uint32_t* components, const bool flipY) const
{
	int x, y, channels;

	stbi_set_flip_vertically_on_load(flipY);
	unsigned char* data = stbi_load(filepath, &x, &y, &channels, 0);

	LOG_CATEGORY_ASSERT(data, "Engine", "Could not load image \"{0}\"!", filepath);

	*width = x;
	*height = y;
	*components = channels;

	return data;
}

unsigned char* Image::Load(const std::string& filepath, uint32_t* width, uint32_t* height, uint32_t* components, const bool flipY) const
{
	return Load(filepath.c_str(), width, height, components, flipY);
}
