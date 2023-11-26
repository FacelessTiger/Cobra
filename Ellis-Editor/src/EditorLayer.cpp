#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include <Ellis/UI/UI.h>
#include <Ellis/Scene/SceneSerializer.h>
#include <Ellis/Utils/PlatformUtils.h>
#include <Ellis/Math/Math.h>
#include <Ellis/Scripting/ScriptEngine.h>
#include <Ellis/Scripting/ScriptGlue.h>
#include <Ellis/Renderer/Font.h>
#include <Ellis/Asset/TextureImporter.h>
#include <Ellis/Asset/SceneImporter.h>
#include <Ellis/Asset/AssetManager.h>

namespace Ellis {

	static Ref<Font> s_Font;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{ 
		s_Font = Font::GetDefault();
	}

	void EditorLayer::OnAttach()
	{
		EL_PROFILE_FUNCTION();

		m_IconPlay = TextureImporter::LoadTexture2D("Resources/Icons/PlayButton.png");
		m_IconPause = TextureImporter::LoadTexture2D("Resources/Icons/PauseButton.png");
		m_IconSimulate = TextureImporter::LoadTexture2D("Resources/Icons/SimulateButton.png");
		m_IconStep = TextureImporter::LoadTexture2D("Resources/Icons/StepButton.png");
		m_IconStop = TextureImporter::LoadTexture2D("Resources/Icons/StopButton.png");
		m_IconLogo = TextureImporter::LoadTexture2D("Resources/Icons/Logo.png");
		m_IconMinimize = TextureImporter::LoadTexture2D("Resources/Icons/Minimize.png");
		m_IconMaximize = TextureImporter::LoadTexture2D("Resources/Icons/Maximize.png");
		m_IconRestore = TextureImporter::LoadTexture2D("Resources/Icons/Restore.png");
		m_IconClose = TextureImporter::LoadTexture2D("Resources/Icons/Close.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 300;
		fbSpec.Height = 169;
		m_CameraPreviewFramebuffer = Framebuffer::Create(fbSpec);

		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			// TODO: prompt the user to select a directory
			// NewProject();

			// if no project is open close Ellis
			if (!OpenProject())
				Application::Get().Close();
		}

		m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		Renderer2D::SetLineWidth(4.0f);
	}

	void EditorLayer::OnDetach()
	{
		EL_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		EL_PROFILE_FUNCTION();

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		// Resize
		FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		// Update scene
		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				m_EditorCamera.OnUpdate(ts);
				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}

			case SceneState::Simulate:
			{
				m_EditorCamera.OnUpdate(ts);
				m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
				break;
			}

			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(ts);
				break;
			}
		}

		auto[mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		ScriptGlue::SetEditorViewportSize(viewportSize);
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			ScriptGlue::SetEditorViewportCursorPos({ mouseX, mouseY });
			m_HoveredEntity = (pixelData == -1) ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		OnOverlayRender();

		m_Framebuffer->Unbind();

		m_CameraPreviewFramebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_ActiveScene->RenderRunningScene();
		m_CameraPreviewFramebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		EL_PROFILE_FUNCTION();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		const bool isMaximized = Application::Get().GetWindow().IsMaximized();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("DockSpace Demo", nullptr, window_flags);
		ImGui::PopStyleColor();

		ImGui::PopStyleVar(4);

		{
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(50, 50, 50, 255));
			// Draw window border if the window is not maximized
			if (!isMaximized)
				UI::RenderWindowOuterBorders(ImGui::GetCurrentWindow());

			ImGui::PopStyleColor(); // ImGuiCol_Border
		}
		
		ImGui::SetCursorPosY(UI_Titlebar());

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 400.0f;
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		style.WindowMinSize.x = minWinSizeX;

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel->OnImGuiRender();

		ImGui::Begin("Stats");

#if 0
		std::string name = "None";
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());
#endif

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);

		uint32_t texturePreviewID = m_CameraPreviewFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)texturePreviewID, { 300, 169 }, { 0, 1 }, { 1, 0 });

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				AssetHandle handle = *(AssetHandle*)payload->Data;
				OpenScene(handle);
			}

			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera
			// Runtime camera from entity
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = m_ActiveScene->GetWorldSpaceTransformMatrix(selectedEntity);

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = (m_GizmoType == ImGuizmo::OPERATION::ROTATE) ? 45.0f : 0.5f; // snap to 45 degrees for rotation and 0.5m for translation/scale

			float snapValues[3] = { snapValue, snapValue, snapValue };
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	float EditorLayer::UI_Titlebar()
	{
		Window& window = Application::Get().GetWindow();

		const float titlebarHeight = 58.0f;
		const bool isMaximized = window.IsMaximized();
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

			fgDrawList->AddImage((ImTextureID)(uint64_t)m_IconLogo->GetRendererID(), logoRectStart, logoRectMax, ImVec2(0, 1), ImVec2(1, 0));
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

		window.SetTitlebarHovered(ImGui::IsItemHovered());
		if (isMaximized)
		{
			float windowMousePosY = ImGui::GetMousePos().y - ImGui::GetCursorScreenPos().y;
			if (windowMousePosY >= 0.0f && windowMousePosY <= 5.0f)
				window.SetTitlebarHovered(true); // Account for the top-most pixels which don't register
		}

		// Draw menu bar
		ImGui::SuspendLayout();
		{
			ImGui::SetNextItemAllowOverlap();
			const float logoHorizontalOffset = 16.0f * 2.0f + 48.0f + windowPadding.x;
			ImGui::SetCursorPos(ImVec2(logoHorizontalOffset, 6.0f + titlebarVerticalOffset));
			UI_Menubar();

			if (ImGui::IsItemHovered()) window.SetTitlebarHovered(false);
		}
		ImGui::ResumeLayout();

		// Centered window title
		{
			const char* title = Application::Get().GetSpecification().Name.c_str();

			ImVec2 currentCursorPos = ImGui::GetCursorPos();
			ImVec2 textSize = ImGui::CalcTextSize(title);

			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - textSize.x * 0.5f, 2.0f + windowPadding.y + 6.0f));
			ImGui::Text("%s", title); // Draw title
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
			const int iconWidth = m_IconMinimize->GetWidth();
			const int iconHeight = m_IconMinimize->GetHeight();
			const float padY = (buttonHeight - (float)iconHeight) / 2.0f;

			if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
				window.Minimize();

			UI::DrawButtonImage(m_IconMinimize, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
		}

		// Maximize Button
		ImGui::Spring(-1.0f, 17.0f);
		UI::ShiftCursorY(8.0f);

		{
			const int iconWidth = m_IconMaximize->GetWidth();
			const int iconHeight = m_IconMaximize->GetHeight();

			if (ImGui::InvisibleButton("Maximize", ImVec2(buttonWidth, buttonHeight)))
			{
				if (isMaximized)
					window.Restore();
				else
					window.Maximize();
			}

			UI::DrawButtonImage(isMaximized ? m_IconRestore : m_IconMaximize, buttonColN, buttonColH, buttonColP);
		}

		// Close Button
		ImGui::Spring(-1.0f, 15.0f);
		UI::ShiftCursorY(8.0f);

		{
			const int iconWidth = m_IconClose->GetWidth();
			const int iconHeight = m_IconClose->GetHeight();

			if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight)))
				Application::Get().Close();

			UI::DrawButtonImage(m_IconClose, IM_COL32(192, 192, 192, 255), IM_COL32(255, 255, 255, 255), buttonColP);
		}

		ImGui::Spring(-1.0f, 18.0f);
		ImGui::EndHorizontal();

		return titlebarHeight;
	}

	void EditorLayer::UI_Menubar()
	{
		const ImRect menuBarRect = { ImGui::GetCursorPos(), { ImGui::GetContentRegionAvail().x + ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing() } };

		ImGui::BeginGroup();
		if (UI::BeginMenubar(menuBarRect))
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O")) OpenProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N")) NewScene();
				if (ImGui::MenuItem("Save", "Ctrl+S")) SaveScene();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveSceneAs();
				if (ImGui::MenuItem("Exit")) Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R")) ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}
		}

		UI::EndMenubar();
		ImGui::EndGroup();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 2 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f });
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { buttonActive.x, buttonActive.y, buttonActive.z, 0.5f });

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		if (hasPlayButton)
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;

			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), { size, size }, { 0, 0 }, { 1, 1 }, 0, { 0, 0, 0, 0 }, tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}

		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();

			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), { size, size }, { 0, 0 }, { 1, 1 }, 0, { 0, 0, 0, 0 }, tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}

		if (hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();

			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)(uint64_t)m_IconPause->GetRendererID(), { size, size }, { 0, 0 }, { 1, 1 }, 0, { 0, 0, 0, 0 }, tintColor) && toolbarEnabled)
			{
				m_ActiveScene->SetPause(!isPaused);
			}

			// Step button
			if (isPaused)
			{
				ImGui::SameLine();
				if (ImGui::ImageButton((ImTextureID)(uint64_t)m_IconStep->GetRendererID(), { size, size }, { 0, 0 }, { 1, 1 }, 0, { 0, 0, 0, 0 }, tintColor) && toolbarEnabled)
				{
					m_ActiveScene->Step(1);
				}
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		if (m_SceneState == SceneState::Edit)
			m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(EL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<KeyTypedEvent>(EL_BIND_EVENT_FN(EditorLayer::OnKeyTyped));
		dispatcher.Dispatch<MouseButtonPressedEvent>(EL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<WindowDropEvent>(EL_BIND_EVENT_FN(EditorLayer::OnWindowDrop));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		ScriptEngine::OnKeyPressed(e);

		// Shortcuts
		if (e.IsRepeat()) return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();
				break;
			}

			case Key::O:
			{
				if (control)
					OpenProject();
				break;
			}

			case Key::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}

				break;
			}

			case Key::D:
			{
				if (control)
					OnDuplicateEntity();
				break;
			}

			// Gizmo
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (control)
				{
					ScriptEngine::ReloadAssembly();
				}
				else
				{
					if (!ImGuizmo::IsUsing())
						m_GizmoType = ImGuizmo::OPERATION::SCALE;
				}

				break;
			}
			case Key::Delete:
			{
				if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
				{
					Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
					if (selectedEntity)
					{
						m_SceneHierarchyPanel.SetSelectedEntity({});
						m_ActiveScene->DestroyEntity(selectedEntity);
					}
				}
				break;
			}
		}

		return false;
	}

	bool EditorLayer::OnKeyTyped(KeyTypedEvent& e)
	{
		ScriptEngine::OnKeyTyped(e);

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft && m_SceneState != SceneState::Play)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}

		return false;
	}

	bool EditorLayer::OnWindowDrop(WindowDropEvent& e)
	{
		// TODO: if a project is dropped in, probably open it
		// AssetManager::ImportAsset();

		return true;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			if (!camera)
				return;

			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{
			// Calculate z index for translation
			float zIndex = 0.001f;
			glm::vec3 cameraForwardDirection = m_EditorCamera.GetForwardDirection();
			glm::vec3 projectionCollider = cameraForwardDirection * glm::vec3(zIndex);

			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(tc.Translation.x, tc.Translation.y, 0)) 
										* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0, 0, 1)) 
										* glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, -projectionCollider.z))
										* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			// Circle Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
										* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0, 0, 1))
										* glm::translate(glm::mat4(1.0f), glm::vec3(cc2d.Offset, -projectionCollider.z))
										* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.05f);
				}
			}
		}

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			const TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
			Renderer2D::DrawRect(m_ActiveScene->GetWorldSpaceTransformMatrix(selectedEntity), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewProject()
	{
		Project::New();
	}

	bool EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Ellis Project (*.eproj)\0*.eproj\0");
		if (filepath.empty())
			return false;

		OpenProject(filepath);
		return true;
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			ScriptEngine::Init();

			AssetHandle startScene = Project::GetActive()->GetConfig().StartScene;
			if (startScene)
				OpenScene(startScene);

			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>(Project::GetActive());
		}
	}

	void EditorLayer::SaveProject()
	{
		// Project::SaveActive();
	}

	void EditorLayer::NewScene()
	{
		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		//std::string filepath = FileDialogs::OpenFile("Ellis Scene (*.ellis)\0*.ellis\0");
		//if (!filepath.empty())
		//	OpenScene(filepath);
	}

	void EditorLayer::OpenScene(AssetHandle handle)
	{
		EL_CORE_ASSERT(handle);

		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		Ref<Scene> readOnlyScene = AssetManager::GetAsset<Scene>(handle);
		Ref<Scene> newScene = Scene::Copy(readOnlyScene);

		m_EditorScene = newScene;
		m_SceneHierarchyPanel.SetContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = Project::GetActive()->GetEditorAssetManager()->GetFilePath(handle);
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Ellis Scene (*.ellis)\0*.ellis\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneImporter::SaveScene(scene, path);
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();

		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		EL_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate);

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;

		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_ActiveScene->SetPause(true);
	}

	void Ellis::EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			Entity newEntity = m_EditorScene->DuplicateEntity(selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity(newEntity);
		}
	}

}