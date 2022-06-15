#pragma once

#include <Ellis.h>
#include <Ellis/Scene/Scene.h>
#include <Ellis/Renderer/EditorCamera.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

namespace Ellis {

	class EditorLayer : public Layer
	{
	private:
		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene;

		Entity m_HoveredEntity;
		EditorCamera m_EditorCamera;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = -1;
		std::filesystem::path m_EditorScenePath;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);
	};

}