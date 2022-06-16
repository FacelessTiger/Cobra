#pragma once

#include "Ellis/Core/UUID.h"
#include "Ellis/Core/Timestep.h"
#include "Ellis/Renderer/EditorCamera.h"

#include <entt.hpp>

class b2World;

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

		b2World* m_PhysicsWorld = nullptr;
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	};

}