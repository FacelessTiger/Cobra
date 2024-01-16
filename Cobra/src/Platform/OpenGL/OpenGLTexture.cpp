#include "cbpch.h"
#include "OpenGLTexture.h"

namespace Cobra {

	namespace Utils {

		static GLenum CobraImageFormatToGLDataFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB8: return GL_RGB; break;
				case ImageFormat::RGBA8: return GL_RGBA; break;
			}

			CB_CORE_ASSERT(false);
			return 0;
		}

		static GLenum CobraImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGB8: return GL_RGB8; break;
				case ImageFormat::RGBA8: return GL_RGBA8; break;
			}

			CB_CORE_ASSERT(false);
			return 0;
		}

	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification, Buffer data)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		CB_PROFILE_FUNCTION();

		m_InternalFormat = Utils::CobraImageFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::CobraImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (data)
			SetData(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		CB_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(Buffer data)
	{
		CB_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		CB_CORE_ASSERT(data.Size == (m_Width * m_Height * bpp), "Data must be entire texture!");

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		CB_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

}