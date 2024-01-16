#pragma once

#include "Cobra/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Cobra {

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