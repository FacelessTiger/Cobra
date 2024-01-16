#include "cbpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Cobra {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CB_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		CB_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CB_CORE_ASSERT(status, "Failed to initialize Glad!");

		CB_CORE_INFO("OpenGL Info:");
		CB_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CB_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CB_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	#ifdef CB_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CB_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Cobra requires at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		CB_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}