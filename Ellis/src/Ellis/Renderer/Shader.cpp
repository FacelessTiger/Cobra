#include "elpch.h"
#include "Shader.h"

#include "Ellis/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Ellis {

	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:     EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:   return new OpenGLShader(vertexSource, fragmentSource);
		}

		EL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}