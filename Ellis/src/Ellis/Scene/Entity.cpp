#include "elpch.h"
#include "Entity.h"

namespace Ellis {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{ }

	void Entity::AddChild(Entity child)
	{
		RelationshipComponent& parentRelationship = GetComponent<RelationshipComponent>();
		RelationshipComponent& childRelationship = child.GetComponent<RelationshipComponent>();

		if (std::count(childRelationship.Children.begin(), childRelationship.Children.end(), GetUUID()))
		{
			return;
		}

		if (childRelationship.Parent.has_value())
		{
			Entity oldParent = m_Scene->GetEntityByUUID(*childRelationship.Parent);
			auto& oldParentChildren = oldParent.GetComponent<RelationshipComponent>().Children;

			oldParentChildren.erase(std::remove(oldParentChildren.begin(), oldParentChildren.end(), child.GetUUID()));
		}

		parentRelationship.Children.push_back(child.GetUUID());
		childRelationship.Parent = GetUUID();
	}

	void Entity::RemoveParent()
	{
		RelationshipComponent& childRelationship = GetComponent<RelationshipComponent>();

		if (!childRelationship.Parent.has_value())
			return;

		auto& parentChildren = m_Scene->GetEntityByUUID(*childRelationship.Parent).GetComponent<RelationshipComponent>().Children;
		parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), GetUUID()));
		childRelationship.Parent.reset();
	}

	glm::mat4 Entity::GetRelativeTranslation()
	{
		RelationshipComponent& relationship = GetComponent<RelationshipComponent>();
		TransformComponent& transform = GetComponent<TransformComponent>();

		if (!relationship.Parent)
		{
			return transform.GetTransform();
		}
		else
		{
			// parent transform * this transform
			return m_Scene->GetEntityByUUID(*relationship.Parent).GetComponent<TransformComponent>().GetTransform() * transform.GetTransform();
		}
	}

}