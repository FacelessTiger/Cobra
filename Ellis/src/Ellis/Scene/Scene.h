#pragma once

#include <entt.hpp>

#include "Ellis/Core/Timestep.h"

namespace Ellis {

	class Entity;
	class SceneHierarchyPanel;
	class SceneSerializer;

	class Scene
	{
	private:
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	};

}