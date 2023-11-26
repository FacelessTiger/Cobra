#pragma once

#include <Ellis/Core/Core.h>
#include <Ellis/Core/Log.h>
#include <Ellis/Scene/Scene.h>
#include <Ellis/Scene/Entity.h>

namespace Ellis {

	class SceneHierarchyPanel
	{
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	};

}