#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Cobra {

	Image::Image(const std::vector<uint8_t>& bytes)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);
		uint8_t* data = stbi_load_from_memory(bytes.data(), bytes.size(), &width, &height, &channels, 4);

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

}