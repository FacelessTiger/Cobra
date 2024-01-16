#pragma once

#include <vector>
#include <glad/glad.h>

namespace Cobra {

	class Image
	{
	public:
		Image(const std::vector<uint8_t>& bytes);

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		inline uint32_t GetRendererID() const { return m_RendererID; }
	private:
		uint32_t m_Width;
		uint32_t m_Height;

		uint32_t m_RendererID;
	};

}