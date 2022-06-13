#pragma once

#include <entt.hpp>

#include "Ellis/Core/Timestep.h"

namespace Ellis {

	class Entity;

	class Scene
	{
	private:
		friend class Entity;

		entt::registry m_Registry;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);
	};

}