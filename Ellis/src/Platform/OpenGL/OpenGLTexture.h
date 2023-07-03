#pragma once

#include <Ellis/Renderer/Texture.h>

#include <glad/glad.h>

namespace Ellis {

	class OpenGLTexture2D : public Texture2D
	{
	private:
		TextureSpecification m_Specification;

		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		
		GLenum m_InternalFormat, m_DataFormat;
	public:
		OpenGLTexture2D(const TextureSpecification& specification, Buffer data = Buffer());
		virtual ~OpenGLTexture2D();

		const TextureSpecification& GetSpecification() const override { return m_Specification; }

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		uint32_t GetRendererID() const override { return m_RendererID; }

		void SetData(Buffer data) override;

		void Bind(uint32_t slot = 0) const override;

		bool IsLoaded() const override { return m_IsLoaded; }

		bool operator==(const Texture& other) const override 
		{ 
			return m_RendererID == other.GetRendererID();
		}
	};

}