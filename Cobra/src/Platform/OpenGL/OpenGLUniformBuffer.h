#pragma once

#include "Cobra/Renderer/UniformBuffer.h"

namespace Cobra {

	class OpenGLUniformBuffer : public UniformBuffer
	{
	private:
		uint32_t m_RendererID = 0;
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	};
}