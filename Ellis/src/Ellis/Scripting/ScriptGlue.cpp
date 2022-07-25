#include "elpch.h"
#include "ScriptGlue.h"

#include "Ellis/Scripting/ScriptEngine.h"
#include "Ellis/Core/KeyCodes.h"
#include "Ellis/Core/Input.h"

#include <mono/metadata/object.h>

namespace Ellis {

#define EL_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ellis.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);

		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		EL_CORE_WARN("Value: {0}", *parameter);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		EL_CORE_WARN("Value: {0}", *parameter);
		return glm::dot(*parameter, *parameter);
	}

	static void Entity_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void Entity_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	void ScriptGlue::RegisterFunctions()
	{
		EL_ADD_INTERNAL_CALL(NativeLog);
		EL_ADD_INTERNAL_CALL(NativeLog_Vector);
		EL_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		EL_ADD_INTERNAL_CALL(Entity_GetTranslation);
		EL_ADD_INTERNAL_CALL(Entity_SetTranslation);

		EL_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}

}