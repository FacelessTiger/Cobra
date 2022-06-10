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

	#ifdef EL_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		EL_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Ellis requires at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}