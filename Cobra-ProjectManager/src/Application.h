#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <string>
#include <vector>
#include <memory>

#include "UI/Image.h"

namespace Cobra {

	struct Project
	{
		std::string Name;
		std::string Path;

		bool operator==(const Project& other) const
		{
			return other.Name == Name && other.Path == Path;
		}
	};

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		inline bool GetTitlebarHovered() const { return m_TitlebarHovered; }
	private:
		void SetDarkThemeColors();

		void RenderImGui();
		void DrawSelectionPanel();
		void DrawTopPanel();
		void DrawBottomPanel();

		void OpenProject();
		void SerializeProjects();
		void DeserializeProjects();

		float UI_Titlebar();
	private:
		GLFWwindow* m_Window;
		uint32_t m_Width = 1600;
		uint32_t m_Height = 900;
		std::string m_WindowTitle = "Cobra Project Manager";

		std::string m_OpenPath;
		bool m_IsMaximized = false;
		bool m_OpenMode = true;
		bool m_TitlebarHovered = false;

		std::string m_CreateName;
		std::string m_CreateFolder;

		std::vector<Project> m_Projects;

		std::shared_ptr<Image> m_CloseIcon, m_MinimizeIcon, m_MaximizeIcon, m_RestoreIcon, m_LogoIcon;
	};

}