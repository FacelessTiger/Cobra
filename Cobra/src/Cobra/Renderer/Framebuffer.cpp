#include "cbpch.h"
#include "Framebuffer.h"

#include "Cobra/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Cobra {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:     CB_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:   return CreateRef<OpenGLFramebuffer>(spec);
		}

		CB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}