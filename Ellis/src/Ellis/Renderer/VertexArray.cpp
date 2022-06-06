#include "elpch.h"
#include "VertexArray.h"

#include "Ellis/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Ellis {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:     EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::OpenGL:   return new OpenGLVertexArray();
		}

		EL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}