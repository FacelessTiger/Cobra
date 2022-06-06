#pragma once

#include "RendererAPI.h"

namespace Ellis {

	class RenderCommand
	{
	private:
		static RendererAPI* s_RendererAPI;
	public:
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }
	};

}