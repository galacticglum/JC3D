#include <Renderer/Texture.h>

TextureWrap Texture::s_WrapMode = TextureWrap::Clamp;
TextureFilter Texture::s_FilterMode = TextureFilter::Linear;

uint32_t Texture::GetStride(const TextureFormat textureFormat)
{
	switch (textureFormat)
	{
		case TextureFormat::RGB: return 3;
		case TextureFormat::RGBA: return 4;
		case TextureFormat::LuminanceAlpha: return 4;
	}

	return 0;
}
