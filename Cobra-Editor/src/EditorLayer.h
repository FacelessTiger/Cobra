#pragma once

#include <Cobra.h>
#include <Cobra/Scene/Scene.h>
#include <Cobra/Renderer/EditorCamera.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Cobra {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyTyped(KeyTypedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnWindowDrop(WindowDropEvent& e);
		void OnOverlayRender();

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();

		void NewScene();
		void OpenScene();
		void OpenScene(AssetHandle handle);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneSimulate();
		void OnSceneStop();
		void OnScenePause();

		void OnDuplicateEntity();

		// UI Panels
		float UI_Titlebar();
		void UI_Menubar();
		void UI_Toolbar();
	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Framebuffer> m_CameraPreviewFramebuffer;
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		Entity m_HoveredEntity;
		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = -1;
		bool m_ShowPhysicsColliders = false;
		std::filesystem::path m_EditorScenePath;

		enum class SceneState { Edit = 0, Play = 1, Simulate = 2 };
		SceneState m_SceneState = SceneState::Edit;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;

		// Editor resources
		Ref<Texture2D> m_IconPlay, m_IconPause, m_IconSimulate, m_IconStep, m_IconStop, m_IconLogo, m_IconMinimize, m_IconMaximize, m_IconClose, m_IconRestore;
	};

}