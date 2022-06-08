#pragma once

#include "Ellis/Renderer/RendererAPI.h"

namespace Ellis {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() const override;

		void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}