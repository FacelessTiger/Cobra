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
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}