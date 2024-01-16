#include "Application.h"
#include "Utils/PlatformUtils.h"
#include "UI/UI.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <filesystem>
#include <fstream>

#include "UI/EmbeddedImages.h"

namespace Cobra {

	Application::Application()
	{
		// Init glfw and glad
		glfwInit();

		glfwWindowHint(GLFW_TITLEBAR, false);
		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowTitle.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glfwSetWindowUserPointer(m_Window, this);
		glfwSetTitlebarHitTestCallback(m_Window, [](GLFWwindow* window, int x, int y, int* hit)
		{
			Application& data = *(Application*)glfwGetWindowUserPointer(window);
			*hit = data.GetTitlebarHovered();
		});

		// init imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		//io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		DeserializeProjects();

		// Embedded icons
		m_CloseIcon = std::make_shared<Image>(g_WindowCloseIcon);
		m_MinimizeIcon = std::make_shared<Image>(g_WindowMinimizeIcon);
		m_MaximizeIcon = std::make_shared<Image>(g_WindowMaximizeIcon);
		m_RestoreIcon = std::make_shared<Image>(g_WindowRestoreIcon);
		m_LogoIcon = std::make_shared<Image>(g_LogoIcon);
	}

	void Application::Run()
	{
		while (!glfwWindowShouldClose(m_Window))
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Window);

			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			RenderImGui();

			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(m_Width, m_Height);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}
	}

	void Application::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = { 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = { 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = { 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = { 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = { 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = { 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = { 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = { 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = { 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = { 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = { 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = { 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = { 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = { 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = { 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = { 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = { 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = { 0.15f, 0.1505f, 0.151f, 1.0f };
	}

	void Application::RenderImGui()
	{
		m_IsMaximized = (bool)glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, m_IsMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		ImGui::PopStyleColor();

		ImGui::PopStyleVar(4);

		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
			// Draw window border if the window is not maximized
			if (!m_IsMaximized)
				UI::RenderWindowOuterBorders(ImGui::GetCurrentWindow());

			ImGui::PopStyleColor(); // ImGuiCol_Border
		}

		ImGui::SetCursorPosY(UI_Titlebar());

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		//style.WindowMinSize.x = 400.0f;
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		style.WindowMinSize.x = minWinSizeX;

		DrawSelectionPanel();
		DrawTopPanel();
		DrawBottomPanel();

		ImGui::End();
	}

	void Application::DrawSelectionPanel()
	{
		ImGui::Begin("##selection panel");

		float width = ImGui::GetWindowWidth();
		if (ImGui::Button("Open", { width, width }))
			m_OpenMode = true;

		if (ImGui::Button("Create", { width, width }))
			m_OpenMode = false;

		ImGui::End();
	}

	void Application::DrawTopPanel()
	{
		ImGui::Begin("##toppanel");

		if (m_OpenMode)
		{
			if (ImGui::BeginTable("##projectstable", 2, ImGuiTableFlags_Resizable))
			{
				ImGui::TableSetupColumn("Name");
				ImGui::NextColumn();
				ImGui::TableSetupColumn("Path");
				ImGui::TableHeadersRow();

				ImGui::TableSetColumnIndex(0);
				for (auto& project : m_Projects)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text(project.Name.c_str());
					ImGui::TableNextColumn();
					ImGui::Text(project.Path.c_str());

					ImGui::SameLine();
					if (ImGui::Selectable(("##selectable" + project.Path).c_str(), false, ImGuiSelectableFlags_SpanAllColumns))
					{
						m_OpenPath = project.Path;
						OpenProject();
						std::exit(0);
					}
				}

				ImGui::EndTable();
			}
		}
		else
		{
			char nameBuffer[126];
			strcpy_s(nameBuffer, sizeof(nameBuffer), m_CreateName.c_str());

			char folderBuffer[126];
			strcpy_s(folderBuffer, sizeof(folderBuffer), m_CreateFolder.c_str());

			ImGui::Text("Project Name");
			ImGui::SameLine();

			if (ImGui::InputText("##name", nameBuffer, 126))
				m_CreateName = nameBuffer;

			ImGui::Text("Containing folder");
			ImGui::SameLine();

			if (ImGui::InputText("##folder", folderBuffer, 126))
				m_CreateFolder = folderBuffer;

			ImGui::SameLine();
			if (ImGui::Button("##button", ImVec2(20, 20)))
				m_CreateFolder = FileDialogs::OpenFolder(m_Window);

			if (ImGui::Button("Create"))
			{

			}
		}

		ImGui::End();
	}

	void Application::DrawBottomPanel()
	{
		ImGui::Begin("##bottompanel");

		if (m_OpenMode)
		{
			bool pathValid = false;
			std::string errorMessage;

			if (std::filesystem::exists(m_OpenPath))
			{
				std::filesystem::path path(m_OpenPath);
				if (path.extension() == ".eproj")
					pathValid = true;
				else
					errorMessage = "File must have .eproj extension!";
			}
			else
			{
				errorMessage = "Invalid path!";
			}

			if (ImGui::Button("Open") && pathValid)
			{
				std::filesystem::path path(m_OpenPath);

				Project project;
				project.Name = path.stem().string();
				project.Path = m_OpenPath;

				if (std::find(m_Projects.begin(), m_Projects.end(), project) == m_Projects.end())
				{
					m_Projects.push_back(project);
					SerializeProjects();
				}

				OpenProject();
				std::exit(0);
			}

			if (!pathValid) ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));

			char buffer[126];
			strcpy_s(buffer, sizeof(buffer), m_OpenPath.c_str());

			ImGui::SameLine();
			if (ImGui::InputText("##opentext", buffer, 126))
			{
				m_OpenPath = buffer;
			}

			ImGui::SameLine();
			if (ImGui::Button("##button", ImVec2(20, 20)))
				m_OpenPath = FileDialogs::OpenFile("Cobra Project (*.eproj)\0*.eproj\0", m_Window);

			ImGui::Text(errorMessage.c_str());
			if (!pathValid) ImGui::PopStyleColor();
		}

		ImGui::End();
	}

	void Application::OpenProject()
	{
		glfwDestroyWindow(m_Window);

		std::string basePath = std::getenv("COBRA_DIR");

		std::filesystem::current_path(basePath + "\\Cobra-Editor");
		std::string runEditor = "\"\"" + basePath + +"\\bin\\" + CONFIG_FOLDER_NAME + "\\Cobra-Editor\\Cobra-Editor.exe\"" + " \"" + m_OpenPath + "\"\"";
		std::cout << runEditor << std::endl;

		system(runEditor.c_str());
	}

	void Application::SerializeProjects()
	{
		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "Projects" << YAML::Value;
			out << YAML::BeginSeq;

			for (auto& project : m_Projects)
			{
				out << YAML::BeginMap; // Projects
				out << YAML::Key << "Name" << YAML::Value << project.Name;
				out << YAML::Key << "Path" << YAML::Value << project.Path;
				out << YAML::EndMap; // Projects
			}

			out << YAML::EndSeq;
			out << YAML::EndMap; // Root
		}

		std::ofstream fout("projects.yaml");
		fout << out.c_str();
	}

	void Application::DeserializeProjects()
	{
		std::filesystem::path path("projects.yaml");
		if (std::filesystem::exists(path))
		{
			YAML::Node data;
			try
			{
				data = YAML::LoadFile(path.string());
			}
			catch (YAML::ParserException e)
			{
				std::cerr << "Failed to load projects file!" << std::endl;
				return;
			}

			auto projectsNode = data["Projects"];
			if (!projectsNode)
				return;

			for (auto& projectNode : projectsNode)
			{
				Project project;
				project.Name = projectNode["Name"].as<std::string>();
				project.Path = projectNode["Path"].as<std::string>();

				m_Projects.push_back(project);
			}
		}
	}

	float Application::UI_Titlebar()
	{
		const float titlebarHeight = 58.0f;
		const bool isMaximized = m_IsMaximized;
		float titlebarVerticalOffset = isMaximized ? -6.0f : 0.0f;
		const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
		const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
									 ImGui::GetCursorScreenPos().y + titlebarHeight };
		auto* bgDrawList = ImGui::GetBackgroundDrawList();
		auto* fgDrawList = ImGui::GetForegroundDrawList();
		bgDrawList->AddRectFilled(titlebarMin, titlebarMax, IM_COL32(21, 21, 21, 255));

		// DEBUG TITLEBAR BOUNDS
		//fgDrawList->AddRect(titlebarMin, titlebarMax, IM_COL32(255, 255, 255, 255));

		// Logo
		{
			const int logoWidth = 48;
			const int logoHeight = 48;

			const ImVec2 logoOffset(16.0f + windowPadding.x, 5.0f + windowPadding.y + titlebarVerticalOffset);
			const ImVec2 logoRectStart = { ImGui::GetItemRectMin().x + logoOffset.x, ImGui::GetItemRectMin().y + logoOffset.y };
			const ImVec2 logoRectMax = { logoRectStart.x + logoWidth, logoRectStart.y + logoHeight };

			fgDrawList->AddImage((ImTextureID)(uint64_t)m_LogoIcon->GetRendererID(), logoRectStart, logoRectMax, ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });

		static float moveOffsetX;
		static float moveOffsetY;
		const float w = ImGui::GetContentRegionAvail().x;
		const float buttonsAreaWidth = 94;

		// Title bar drag area
		ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y + titlebarVerticalOffset));
		// DEBUG DRAG BOUNDS
		// fgDrawList->AddRect(ImGui::GetCursorScreenPos(), ImVec2(ImGui::GetCursorScreenPos().x + w - buttonsAreaWidth, ImGui::GetCursorScreenPos().y + titlebarHeight), IM_COL32(255, 255, 255, 255));
		ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight), ImGuiButtonFlags_AllowOverlap);

		m_TitlebarHovered = ImGui::IsItemHovered();
		if (isMaximized)
		{
			float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
			if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
				m_TitlebarHovered = true; // Account for the top-most pixels which don't register
		}

		// Draw menu bar
		/*ImGui::SuspendLayout();
		{
			ImGui::SetNextItemAllowOverlap();
			const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
			ImGui::SetCursorPos(ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
			UI_Menubar();

			if (ImGui::IsItemHovered()) window.SetTitlebarHovered(false);
		}
		ImGui::ResumeLayout();*/

		// Centered window title
		{
			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize(m_WindowTitle.c_str());

			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", m_WindowTitle.c_str()); // Draw title
			ImGui::SetCursorPos(currentCursorPos);
		}

		// Window buttons
		const ImU32 buttonColN = IM_COL32(172, 172, 172, 255);
		const ImU32 buttonColH = IM_COL32(230, 230, 230, 255);
		const ImU32 buttonColP = IM_COL32(128, 128, 128, 255);
		const float buttonWidth = 14.0f;
		const float buttonHeight = 14.0f;

		// Minimize button
		ImGui::Spring();
		UI::ShiftCursorY(8.0f);

		{
			const int iconWidth = m_MinimizeIcon->GetWidth();
			const int iconHeight = m_MinimizeIcon->GetHeight();
			const float padY = (buttonHeight - (float)iconHeight) / 2.0f;

			if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
				glfwIconifyWindow(m_Window);

			UI::DrawButtonImage(m_MinimizeIcon, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
		}

		// Maximize Button
		ImGui::Spring(-1.0f, 17.0f);
		UI::ShiftCursorY(8.0f);

		{
			if (ImGui::InvisibleButton("Maximize", ImVec2(buttonWidth, buttonHeight)))
			{
				if (isMaximized)
					glfwRestoreWindow(m_Window);
				else
					glfwMaximizeWindow(m_Window);
			}

			UI::DrawButtonImage(isMaximized ? m_RestoreIcon : m_MaximizeIcon, buttonColN, buttonColH, buttonColP);
		}

		// Close Button
		ImGui::Spring(-1.0f, 15.0f);
		UI::ShiftCursorY(8.0f);

		{
			if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
				std::exit(0);
			
			UI::DrawButtonImage(m_CloseIcon, IM_COL32(192, 192, 192, 255), IM_COL32(255, 255, 255, 255), buttonColP);
		}

		ImGui::Spring(-1.0f, 18.0f);
		ImGui::EndHorizontal();

		return titlebarHeight;
	}

}