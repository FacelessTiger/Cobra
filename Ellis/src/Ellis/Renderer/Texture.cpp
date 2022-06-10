#include "elpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Ellis {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:     EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:   return CreateRef<OpenGLTexture2D>(path);
		}

		EL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}