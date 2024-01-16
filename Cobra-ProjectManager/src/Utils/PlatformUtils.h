#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace Cobra {

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string OpenFile(const char* filter, GLFWwindow* window);
		static std::string SaveFile(const char* filter, GLFWwindow* window);

		static std::string OpenFolder(GLFWwindow* window);
	};

	class Time
	{
	public:
		static float GetTime();
	};

}