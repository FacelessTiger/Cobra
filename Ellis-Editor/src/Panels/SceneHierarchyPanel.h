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
	private:
		void DrawEntityNode(Entity entity);
	};

}