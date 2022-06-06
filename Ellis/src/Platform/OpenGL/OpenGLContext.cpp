#include "elpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Ellis {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		EL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EL_CORE_ASSERT(status, "Failed to initialize Glad!");

		EL_CORE_INFO("OpenGL Info:");
		EL_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		EL_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		EL_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}