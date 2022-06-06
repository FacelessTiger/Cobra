#include "elpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ellis {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}