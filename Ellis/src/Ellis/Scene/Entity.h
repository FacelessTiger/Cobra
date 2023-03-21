#pragma once

#include "Ellis/Core/UUID.h"
#include "Ellis/Scene/Scene.h"
#include "Ellis/Scene/Components.h"

#include <entt.hpp>

namespace Ellis {

	class Entity
	{
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	public:
		class EntityChildrenIterator
		{
		private:
			std::vector<UUID>::iterator m_Iterator;
			Scene* m_Scene;
		public:
			EntityChildrenIterator(std::vector<UUID>::iterator iterator, Scene* scene) : m_Iterator(iterator), m_Scene(scene) { }

			EntityChildrenIterator& operator++()
			{
				m_Iterator++;
				return *this;
			}

			EntityChildrenIterator operator++(int)
			{
				EntityChildrenIterator iterator = *this;
				++*this;
				return iterator;
			}

			bool operator!=(const EntityChildrenIterator& iterator) { return m_Iterator != iterator.m_Iterator; }
			Entity operator*() { return m_Scene->GetEntityByUUID(*m_Iterator); }
		};

		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EL_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);

			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			EL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		void AddChild(Entity child);
		void RemoveParent();
		bool HasParent() { return GetComponent<RelationshipComponent>().Parent.has_value(); }
		glm::mat4 GetRelativeTranslation();

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		EntityChildrenIterator begin() { return EntityChildrenIterator(GetComponent<RelationshipComponent>().Children.begin(), m_Scene); }
		EntityChildrenIterator end() { return EntityChildrenIterator(GetComponent<RelationshipComponent>().Children.end(), m_Scene); }

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }
	};

}