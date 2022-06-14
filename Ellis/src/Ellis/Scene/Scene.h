#pragma once

#include <entt.hpp>

#include "Ellis/Core/Timestep.h"

namespace Ellis {

	class Entity;
	class SceneHierarchyPanel;

	class Scene
	{
	private:
		friend class Entity;
		friend class SceneHierarchyPanel;

		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
	};

}