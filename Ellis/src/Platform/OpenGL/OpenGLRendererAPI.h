#pragma once

#include "Ellis/Renderer/RendererAPI.h"

namespace Ellis {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void Init() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void SetClearColor(const glm::vec4& color) override;
		void Clear() const override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}