#pragma once

#include "Ellis/Core/UUID.h"
#include "Ellis/Scene/SceneCamera.h"
#include "Ellis/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Common.h"

namespace Ellis {

	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{ }
	};

	struct TransformComponent
	{
		static constexpr char* Name = "Transform";
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation)
		{ }

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	typedef typename concat<TYPE_LIST, TransformComponent>::type TypeListTransformComponent;
	#undef TYPE_LIST
	#define TYPE_LIST TypeListTransformComponent

	struct SpriteRendererComponent
	{
		static constexpr char* Name = "Sprite Renderer";

		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) 
		{ }
	};

	typedef typename concat<TYPE_LIST, SpriteRendererComponent>::type TypeListSpriteRendererComponent;
	#undef TYPE_LIST
	#define TYPE_LIST TypeListSpriteRendererComponent

	struct CircleRendererComponent
	{
		static constexpr char* Name = "Circle Renderer";

		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f; // TODO: Redesign this, not user friendly and resolution dependant

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	typedef typename concat<TYPE_LIST, CircleRendererComponent>::type TypeListCircleRendererComponent;
	#undef TYPE_LIST
	#define TYPE_LIST TypeListCircleRendererComponent

	struct CameraComponent
	{
		static constexpr char* Name = "Camera";

		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	typedef typename concat<TYPE_LIST, CameraComponent>::type TypeListCameraComponent;
	#undef TYPE_LIST
	#define TYPE_LIST TypeListCameraComponent

	// Forward declaration 
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		static constexpr char* Name = "Native Script";

		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	typedef typename concat<TYPE_LIST, NativeScriptComponent>::type TypeListNativeScriptComponent;
	#undef TYPE_LIST
	#define TYPE_LIST TypeListNativeScriptComponent

	// Physics
	struct Rigidbody2DComponent
	{
		static constexpr char* Name = "Rigidbody 2D";

		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	typedef typename concat<TYPE_LIST, Rigidbody2DComponent>::type TypeListRigidbody2DComponent;
	#undef TYPE_LIST
	#define TYPE_LIST TypeListRigidbody2DComponent

	struct BoxCollider2DComponent
	{
		static constexpr char* Name = "Box Collider 2D";

		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO: move into physics material
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	typedef typename concat<TYPE_LIST, BoxCollider2DComponent>::type TypeListBoxCollider2DComponent;
	#undef TYPE_LIST
	#define TYPE_LIST TypeListBoxCollider2DComponent

	struct CircleCollider2DComponent
	{
		static constexpr char* Name = "Circle Collider 2D";

		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		// TODO: move into physics material
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

	typedef typename concat<TYPE_LIST, CircleCollider2DComponent>::type TypeListCircleCollider2DComponent;
	#undef TYPE_LIST
	#define TYPE_LIST TypeListCircleCollider2DComponent

}