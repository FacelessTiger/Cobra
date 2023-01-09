#include "elpch.h"
#include "ScriptGlue.h"

#include "Ellis/Scripting/ScriptEngine.h"
#include "Ellis/Core/KeyCodes.h"
#include "Ellis/Core/Input.h"
#include "Ellis/Physics/Physics2D.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

#include <box2d/b2_body.h>

namespace Ellis {

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define EL_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ellis.InternalCalls::" #Name, Name)

	#pragma region Entity
	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EL_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetUUID();
	}

	static MonoObject* Entity_GetScriptInstance(UUID entityID)
	{
		return ScriptEngine::GetManagedInstance(entityID);
	}
	#pragma endregion

	#pragma region TransformComponent
	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void TransformComponent_GetRotation(UUID entityID, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID entityID, glm::vec3* rotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Rotation = *rotation;
	}

	static void TransformComponent_GetScale(UUID entityID, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(UUID entityID, glm::vec3* scale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Scale = *scale;
	}
	#pragma endregion

	#pragma region SpriteRendererComponent
	static void SpriteRendererComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		*outColor = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		entity.GetComponent<SpriteRendererComponent>().Color = *color;
	}

	static void SpriteRendererComponent_GetTilingFactor(UUID entityID, float* outTilingFactor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		*outTilingFactor = entity.GetComponent<SpriteRendererComponent>().TilingFactor;
	}

	static void SpriteRendererComponent_SetTilingFactor(UUID entityID, float* tilingFactor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		entity.GetComponent<SpriteRendererComponent>().TilingFactor = *tilingFactor;
	}
	#pragma endregion

	#pragma region CircleRendererComponent
	static void CircleRendererComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		*outColor = entity.GetComponent<CircleRendererComponent>().Color;
	}

	static void CircleRendererComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Color = *color;
	}

	static void CircleRendererComponent_GetThickness(UUID entityID, float* outThickness)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		*outThickness = entity.GetComponent<CircleRendererComponent>().Thickness;
	}

	static void CircleRendererComponent_SetThickness(UUID entityID, float* thickness)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Thickness = *thickness;
	}

	static void CircleRendererComponent_GetFade(UUID entityID, float* outFade)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		*outFade = entity.GetComponent<CircleRendererComponent>().Fade;
	}

	static void CircleRendererComponent_SetFade(UUID entityID, float* fade)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Fade = *fade;
	}
	#pragma endregion

	#pragma region Rigidbody2DComponent
	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* worldPosition, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(worldPosition->x, worldPosition->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void Rigidbody2DComponent_GetLinearVelocity(UUID entityID, glm::vec2* outLinearVelocity)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		const b2Vec2& linearVelocity = body->GetLinearVelocity();
		*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
	}

	static Rigidbody2DComponent::BodyType Rigidbody2DComponent_GetType(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		return Utils::Rigidbody2DTypeFromBox2DBody(body->GetType());
	}

	static void Rigidbody2DComponent_SetType(UUID entityID, Rigidbody2DComponent::BodyType bodyType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EL_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EL_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		body->SetType(Utils::Rigidbody2DTypeToBox2DBody(bodyType));
	}
	#pragma endregion

	#pragma region Input
	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}
	#pragma endregion

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypeName = fmt::format("Ellis.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				EL_CORE_ERROR("Could not find component type {}", managedTypeName);
				return;
			}

			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		EL_ADD_INTERNAL_CALL(Entity_HasComponent);
		EL_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		EL_ADD_INTERNAL_CALL(Entity_GetScriptInstance);

		EL_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		EL_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		EL_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		EL_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		EL_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		EL_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		EL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
		EL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);
		EL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTilingFactor);
		EL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTilingFactor);

		EL_ADD_INTERNAL_CALL(CircleRendererComponent_GetColor);
		EL_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
		EL_ADD_INTERNAL_CALL(CircleRendererComponent_GetThickness);
		EL_ADD_INTERNAL_CALL(CircleRendererComponent_SetThickness);
		EL_ADD_INTERNAL_CALL(CircleRendererComponent_GetFade);
		EL_ADD_INTERNAL_CALL(CircleRendererComponent_SetFade);

		EL_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		EL_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
		EL_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetLinearVelocity);
		EL_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetType);
		EL_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetType);

		EL_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}

}