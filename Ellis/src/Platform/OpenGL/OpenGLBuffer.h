#pragma once

#include "Ellis/Renderer/Buffer.h"

namespace Ellis {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_RendererID;
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		void Bind() const override;
		void Unbind() const override;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		uint32_t GetCount() const override { return m_Count; }
	};

}