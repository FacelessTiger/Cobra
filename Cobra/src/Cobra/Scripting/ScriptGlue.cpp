#include "cbpch.h"
#include "ScriptGlue.h"

#include "Cobra/Scripting/ScriptEngine.h"
#include "Cobra/Core/KeyCodes.h"
#include "Cobra/Core/MouseButtonCodes.h"
#include "Cobra/Core/Input.h"
#include "Cobra/Physics/Physics2D.h"
#include "Cobra/Core/Application.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/appdomain.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <box2d/b2_body.h>
#include <imgui.h>

namespace Cobra {

	namespace Utils {
		std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			
			mono_free(cStr);
			return str;
		}

		MonoArray* Mat4ToMonoArray(const glm::mat4& matrix)
		{
			float* pointer = (float*)glm::value_ptr(matrix);
			MonoArray* ret = ScriptEngine::CreateArray(mono_get_single_class(), 4 * 4);

			for (int i = 0; i < 4 * 4; i++)
				mono_array_set(ret, float, i, pointer[i]);

			return ret;
		}

		glm::mat4 MonoArrayToMat4(MonoArray* array)
		{
			float cppArray[16];

			for (int i = 0; i < 4 * 4; i++)
				cppArray[i] = mono_array_get(array, float, i);

			return glm::make_mat4(cppArray);
		}
	}

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;
	static glm::vec2 s_EditorViewportCursorPos;
	static glm::vec2 s_EditorViewportSize;

#define CB_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Cobra.InternalCalls::" #Name, Name)

	#pragma region Entity
	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		CB_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static void Entity_NativeLog(MonoString* string)
	{
		std::string str = Utils::MonoStringToString(string);
		CB_CORE_INFO("Script: {}", str);
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->FindEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetUUID();
	}

	static uint32_t Entity_GetWindowWidth()
	{
		// TODO: return Application::Get().GetWindow().GetWidth() in runtime
		return s_EditorViewportSize.x;
	}

	static uint32_t Entity_GetWindowHeight()
	{
		// TODO: return Application::Get().GetWindow().GetHeight() in runtime
		return s_EditorViewportSize.y;
	}

	static MonoArray* Entity_GetWorldTransform(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		return Utils::Mat4ToMonoArray(scene->GetWorldSpaceTransformMatrix(entity));
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
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void TransformComponent_GetRotation(UUID entityID, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID entityID, glm::vec3* rotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Rotation = *rotation;
	}

	static void TransformComponent_GetScale(UUID entityID, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(UUID entityID, glm::vec3* scale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Scale = *scale;
	}

	static MonoArray* TransformComponent_GetTransform(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		glm::mat4 transform = entity.GetComponent<TransformComponent>().GetTransform();
		return Utils::Mat4ToMonoArray(transform);
	}
	#pragma endregion

	#pragma region SpriteRendererComponent
	static void SpriteRendererComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outColor = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<SpriteRendererComponent>().Color = *color;
	}

	static void SpriteRendererComponent_GetTilingFactor(UUID entityID, float* outTilingFactor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outTilingFactor = entity.GetComponent<SpriteRendererComponent>().TilingFactor;
	}

	static void SpriteRendererComponent_SetTilingFactor(UUID entityID, float* tilingFactor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<SpriteRendererComponent>().TilingFactor = *tilingFactor;
	}
	#pragma endregion

	#pragma region CircleRendererComponent
	static void CircleRendererComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outColor = entity.GetComponent<CircleRendererComponent>().Color;
	}

	static void CircleRendererComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Color = *color;
	}

	static void CircleRendererComponent_GetThickness(UUID entityID, float* outThickness)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outThickness = entity.GetComponent<CircleRendererComponent>().Thickness;
	}

	static void CircleRendererComponent_SetThickness(UUID entityID, float* thickness)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Thickness = *thickness;
	}

	static void CircleRendererComponent_GetFade(UUID entityID, float* outFade)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outFade = entity.GetComponent<CircleRendererComponent>().Fade;
	}

	static void CircleRendererComponent_SetFade(UUID entityID, float* fade)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<CircleRendererComponent>().Fade = *fade;
	}
	#pragma endregion

	#pragma region CameraComponent
	static MonoArray* CameraComponent_GetProjection(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		glm::mat4 projection = entity.GetComponent<CameraComponent>().Camera.GetProjection();
		return Utils::Mat4ToMonoArray(projection);
	}
	#pragma endregion

	#pragma region Rigidbody2DComponent
	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* worldPosition, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(worldPosition->x, worldPosition->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void Rigidbody2DComponent_GetLinearVelocity(UUID entityID, glm::vec2* outLinearVelocity)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		const b2Vec2& linearVelocity = body->GetLinearVelocity();
		*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
	}

	static Rigidbody2DComponent::BodyType Rigidbody2DComponent_GetType(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		return Utils::Rigidbody2DTypeFromBox2DBody(body->GetType());
	}

	static void Rigidbody2DComponent_SetType(UUID entityID, Rigidbody2DComponent::BodyType bodyType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;

		body->SetType(Utils::Rigidbody2DTypeToBox2DBody(bodyType));
	}
	#pragma endregion

	#pragma region TextComponent
	static MonoString* TextComponent_GetText(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);
		CB_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto& str = entity.GetComponent<TextComponent>().TextString;
		return ScriptEngine::CreateString(str.c_str());
	}

	static void TextComponent_SetText(UUID entityID, MonoString* textString)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);
		CB_CORE_ASSERT(entity.HasComponent<TextComponent>());

		entity.GetComponent<TextComponent>().TextString = Utils::MonoStringToString(textString);
	}

	static void TextComponent_GetKerning(UUID entityID, float* outKerning)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outKerning = entity.GetComponent<TextComponent>().Kerning;
	}

	static void TextComponent_SetKerning(UUID entityID, float* kerning)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<TextComponent>().Kerning = *kerning;
	}

	static void TextComponent_GetLineSpacing(UUID entityID, float* outLineSpacing)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outLineSpacing = entity.GetComponent<TextComponent>().LineSpacing;
	}

	static void TextComponent_SetLineSpacing(UUID entityID, float* lineSpacing)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<TextComponent>().LineSpacing = *lineSpacing;
	}

	static void TextComponent_GetColor(UUID entityID, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		*outColor = entity.GetComponent<TextComponent>().Color;
	}

	static void TextComponent_SetColor(UUID entityID, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CB_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CB_CORE_ASSERT(entity);

		entity.GetComponent<TextComponent>().Color = *color;
	}
	#pragma endregion

	#pragma region Input
	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	static bool Input_IsMouseButtonDown(MouseCode mousecode)
	{
		return Input::IsMouseButtonPressed(mousecode);
	}

	static float Input_GetMouseX()
	{
		// TODO: return Input::GetMouseX() in runtime
		return s_EditorViewportCursorPos.x;
	}

	static float Input_GetMouseY()
	{
		// TODO: return Input::GetMouseY() in runtime
		return s_EditorViewportCursorPos.y;
	}
	#pragma endregion

	#pragma region Matrix4
	static MonoArray* Matrix4_Inverse(MonoArray* array)
	{
		glm::mat4 inversedMatrix = glm::inverse(Utils::MonoArrayToMat4(array));
		return Utils::Mat4ToMonoArray(inversedMatrix);
	}

	static MonoArray* Matrix4_MultiplyMat4Mat4(MonoArray* left, MonoArray* right)
	{
		glm::mat4 leftMatrix = Utils::MonoArrayToMat4(left);
		glm::mat4 rightMatrix = Utils::MonoArrayToMat4(right);

		glm::mat4 ret = leftMatrix * rightMatrix;
		return Utils::Mat4ToMonoArray(ret);
	}

	static glm::vec4 Matrix4_MultiplyMat4Vec4(MonoArray* left, glm::vec4 right)
	{
		glm::mat4 leftMatrix = Utils::MonoArrayToMat4(left);

		return leftMatrix * right;
	}

	static glm::vec3 Matrix4_GetTranslation(MonoArray* array)
	{
		glm::mat4 matrix = Utils::MonoArrayToMat4(array);
		return glm::vec3(matrix[3]);
	}

	static glm::vec3 Matrix4_GetScale(MonoArray* array)
	{
		glm::mat4 matrix = Utils::MonoArrayToMat4(array);

		glm::vec3 scale;
		scale.x = glm::length(glm::vec3(matrix[0]));
		scale.y = glm::length(glm::vec3(matrix[1]));
		scale.z = glm::length(glm::vec3(matrix[2]));

		return glm::vec3(scale);
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
			std::string managedTypeName = fmt::format("Cobra.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType)
			{
				CB_CORE_ERROR("Could not find component type {}", managedTypeName);
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
		CB_ADD_INTERNAL_CALL(Entity_HasComponent);
		CB_ADD_INTERNAL_CALL(Entity_NativeLog);
		CB_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		CB_ADD_INTERNAL_CALL(Entity_GetWindowWidth);
		CB_ADD_INTERNAL_CALL(Entity_GetWindowHeight);
		CB_ADD_INTERNAL_CALL(Entity_GetWorldTransform);
		CB_ADD_INTERNAL_CALL(Entity_GetScriptInstance);

		CB_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		CB_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		CB_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		CB_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		CB_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		CB_ADD_INTERNAL_CALL(TransformComponent_SetScale);
		CB_ADD_INTERNAL_CALL(TransformComponent_GetTransform);

		CB_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
		CB_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);
		CB_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTilingFactor);
		CB_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTilingFactor);

		CB_ADD_INTERNAL_CALL(CircleRendererComponent_GetColor);
		CB_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
		CB_ADD_INTERNAL_CALL(CircleRendererComponent_GetThickness);
		CB_ADD_INTERNAL_CALL(CircleRendererComponent_SetThickness);
		CB_ADD_INTERNAL_CALL(CircleRendererComponent_GetFade);
		CB_ADD_INTERNAL_CALL(CircleRendererComponent_SetFade);

		CB_ADD_INTERNAL_CALL(CameraComponent_GetProjection);

		CB_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		CB_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);
		CB_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetLinearVelocity);
		CB_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetType);
		CB_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetType);

		CB_ADD_INTERNAL_CALL(TextComponent_GetText);
		CB_ADD_INTERNAL_CALL(TextComponent_SetText);
		CB_ADD_INTERNAL_CALL(TextComponent_GetKerning);
		CB_ADD_INTERNAL_CALL(TextComponent_SetKerning);
		CB_ADD_INTERNAL_CALL(TextComponent_GetLineSpacing);
		CB_ADD_INTERNAL_CALL(TextComponent_SetLineSpacing);
		CB_ADD_INTERNAL_CALL(TextComponent_GetColor);
		CB_ADD_INTERNAL_CALL(TextComponent_SetColor);

		CB_ADD_INTERNAL_CALL(Input_IsKeyDown);
		CB_ADD_INTERNAL_CALL(Input_IsMouseButtonDown);
		CB_ADD_INTERNAL_CALL(Input_GetMouseX);
		CB_ADD_INTERNAL_CALL(Input_GetMouseY);

		CB_ADD_INTERNAL_CALL(Matrix4_Inverse);
		CB_ADD_INTERNAL_CALL(Matrix4_MultiplyMat4Mat4);
		CB_ADD_INTERNAL_CALL(Matrix4_MultiplyMat4Vec4);
		CB_ADD_INTERNAL_CALL(Matrix4_GetTranslation);
		CB_ADD_INTERNAL_CALL(Matrix4_GetScale);
	}

	void ScriptGlue::SetEditorViewportCursorPos(const glm::vec2& position)
	{
		s_EditorViewportCursorPos = position;
	}

	void ScriptGlue::SetEditorViewportSize(const glm::vec2& size)
	{
		s_EditorViewportSize = size;
	}

}