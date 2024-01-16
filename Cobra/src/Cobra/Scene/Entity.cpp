#include "cbpch.h"
#include "Entity.h"

namespace Cobra {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{ }

}