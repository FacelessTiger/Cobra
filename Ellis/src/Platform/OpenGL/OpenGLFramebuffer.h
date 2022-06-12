#pragma once

#include "Ellis/Renderer/Framebuffer.h"

namespace Ellis {

	class OpenGLFramebuffer : public Framebuffer
	{
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		void Bind() override;
		void Unbind() override;

		void Resize(uint32_t width, uint32_t height) override;

		uint32_t GetColorAttachmentRendererID() override { return m_ColorAttachment; }
		const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	};

}