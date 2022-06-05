#pragma once

#include "Ellis/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Ellis {

	class OpenGLContext : public GraphicsContext
	{
	private:
		GLFWwindow* m_WindowHandle;
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;
	};

}