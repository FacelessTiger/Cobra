#pragma once

#include "Ellis/Renderer/RenderCommand.h"
#include "Ellis/Renderer/OrthographicCamera.h"
#include "Ellis/Renderer/Shader.h"

namespace Ellis {

	class Renderer
	{
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}