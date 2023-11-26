#include "SceneHierarchyPanel.h"

#include "Ellis/Scripting/ScriptEngine.h"
#include "Ellis/UI/UI.h"
#include "Ellis/Asset/AssetManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>

#include <Ellis/Scene/Components.h>

namespace Ellis {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Context)
		{
			m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity = { entityID, m_Context.get() };
					if (entity.GetComponent<RelationshipComponent>().Parent == -1)
						DrawEntityNode(entity);
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Context->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& relationship = entity.GetComponent<RelationshipComponent>();
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			UUID uuid = entity.GetUUID();

			ImGui::SetDragDropPayload("ENTITY_DRAG", &uuid, sizeof(UUID));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
			{
				UUID uuid = *(UUID*)payload->Data;

				auto& childComponent = m_Context->GetEntityByUUID(uuid).GetComponent<RelationshipComponent>();
				if (childComponent.Parent == -1)
				{
					childComponent.Parent = entity.GetUUID();
					relationship.Children.push_back(uuid);
				}
			}

			ImGui::EndDragDropTarget();
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			if (ImGui::MenuItem("Duplicate Entity"))
				m_Context->DuplicateEntity(entity);

			ImGui::EndPopup();
		}

		if (opened)
		{
			for (const auto& uuid : relationship.Children)
			{
				Entity child = m_Context->GetEntityByUUID(uuid);
				DrawEntityNode(child);
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename Cpptype, ScriptFieldType FieldType, typename Func>
	static void DrawScriptFieldRunning(const ScriptField& field, Ref<ScriptInstance>& scriptInstance, const std::string& name, Func func)
	{
		if (field.Type == FieldType)
		{
			Cpptype data = scriptInstance->GetFieldValue<Cpptype>(name);
			if (func(data))
				scriptInstance->SetFieldValue(name, data);
		}
	}

	template<typename CppType, ScriptFieldType FieldType, typename Func>
	static void DrawScriptFieldSet(const ScriptField& field, ScriptFieldInstance& scriptField, Func func)
	{
		if (field.Type == FieldType)
		{
			CppType data = scriptField.GetValue<CppType>();
			if (func(data))
				scriptField.SetValue(data);
		}
	}

	template<typename CppType, ScriptFieldType FieldType, typename Func>
	static void DrawScriptField(const ScriptField& field, ScriptFieldInstance& fieldInstance, CppType defaultValue, Func func)
	{
		if (field.Type == FieldType)
		{
			CppType data = defaultValue;
			if (func(data))
			{
				fieldInstance.Field = field;
				fieldInstance.SetValue(data);
			}
		}
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

			if (ImGui::Button("+", { lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");
			DisplayAddComponentEntry<TextComponent>("Text");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component)
		{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component)
		{
			auto& camera = component.Camera;
			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
					camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);


				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
		});

		DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable
		{
			bool scriptClassExists = ScriptEngine::EntityClassExists(component.ClassName);

			static char buffer[64];
			strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

			UI::ScopedStyleColor textColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f), !scriptClassExists);

			if (ImGui::InputText("Class", buffer, sizeof(buffer)))
			{
				component.ClassName = buffer;
				return;
			}

			// Fields
			if (scene->IsRunning())
			{
				Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
				if (scriptInstance)
				{
					const auto& fields = scriptInstance->GetScriptClass()->GetFields();

					for (const auto& [name, field] : fields)
					{
						DrawScriptFieldRunning<float, ScriptFieldType::Float>(field, scriptInstance, name, [&name](float& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_Float, &data); });
						DrawScriptFieldRunning<double, ScriptFieldType::Double>(field, scriptInstance, name, [&name](double& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_Double, &data); });
						DrawScriptFieldRunning<bool, ScriptFieldType::Bool>(field, scriptInstance, name, [&name](bool& data) { return ImGui::Checkbox(name.c_str(), &data); });
						DrawScriptFieldRunning<wchar_t, ScriptFieldType::Char>(field, scriptInstance, name, [&name](wchar_t& data) { char sData[2]; std::wcstombs(sData, &data, 1); sData[1] = '\0'; bool ret = ImGui::InputText(name.c_str(), sData, 2); if (ret) std::mbstowcs(&data, sData, 1); return ret; });
						DrawScriptFieldRunning<uint8_t, ScriptFieldType::Byte>(field, scriptInstance, name, [&name](uint8_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U8, &data); });
						DrawScriptFieldRunning<int16_t, ScriptFieldType::Short>(field, scriptInstance, name, [&name](int16_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S16, &data); });
						DrawScriptFieldRunning<int32_t, ScriptFieldType::Int>(field, scriptInstance, name, [&name](int32_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S32, &data); });
						DrawScriptFieldRunning<int64_t, ScriptFieldType::Long>(field, scriptInstance, name, [&name](int64_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S64, &data); });
						DrawScriptFieldRunning<uint16_t, ScriptFieldType::UShort>(field, scriptInstance, name, [&name](uint16_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U16, &data); });
						DrawScriptFieldRunning<uint32_t, ScriptFieldType::UInt>(field, scriptInstance, name, [&name](uint32_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U32, &data); });
						DrawScriptFieldRunning<uint64_t, ScriptFieldType::ULong>(field, scriptInstance, name, [&name](uint64_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U64, &data); });
						DrawScriptFieldRunning<glm::vec2, ScriptFieldType::Vector2>(field, scriptInstance, name, [&name](glm::vec2& data) { return ImGui::DragFloat2(name.c_str(), glm::value_ptr(data)); });
						DrawScriptFieldRunning<glm::vec3, ScriptFieldType::Vector3>(field, scriptInstance, name, [&name](glm::vec3& data) { return ImGui::DragFloat3(name.c_str(), glm::value_ptr(data)); });
						DrawScriptFieldRunning<glm::vec4, ScriptFieldType::Vector4>(field, scriptInstance, name, [&name](glm::vec4& data) { return ImGui::DragFloat4(name.c_str(), glm::value_ptr(data)); });
					}
				}
			}
			else
			{
				if (scriptClassExists)
				{
					Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.ClassName);
					const auto& fields = entityClass->GetFields();
					auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);

					for (const auto& [name, field] : fields)
					{
						// Field has been set in editor
						if (entityFields.find(name) != entityFields.end())
						{
							ScriptFieldInstance& scriptField = entityFields.at(name);

							// Display control to set it
							DrawScriptFieldSet<float, ScriptFieldType::Float>(field, scriptField, [&name](float& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_Float, &data); });
							DrawScriptFieldSet<double, ScriptFieldType::Double>(field, scriptField, [&name](double& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_Double, &data); });
							DrawScriptFieldSet<bool, ScriptFieldType::Bool>(field, scriptField, [&name](bool& data) { return ImGui::Checkbox(name.c_str(), &data); });
							DrawScriptFieldSet<wchar_t, ScriptFieldType::Char>(field, scriptField, [&name](wchar_t& data) { char sData[2]; std::wcstombs(sData, &data, 1); sData[1] = '\0'; bool ret = ImGui::InputText(name.c_str(), sData, 2); if (ret) std::mbstowcs(&data, sData, 1); return ret; });
							DrawScriptFieldSet<uint8_t, ScriptFieldType::Byte>(field, scriptField, [&name](uint8_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U8, &data); });
							DrawScriptFieldSet<int16_t, ScriptFieldType::Short>(field, scriptField, [&name](int16_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S16, &data); });
							DrawScriptFieldSet<int32_t, ScriptFieldType::Int>(field, scriptField, [&name](int32_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S32, &data); });
							DrawScriptFieldSet<int64_t, ScriptFieldType::Long>(field, scriptField, [&name](int64_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S64, &data); });
							DrawScriptFieldSet<uint16_t, ScriptFieldType::UShort>(field, scriptField, [&name](uint16_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U16, &data); });
							DrawScriptFieldSet<uint32_t, ScriptFieldType::UInt>(field, scriptField, [&name](uint32_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U32, &data); });
							DrawScriptFieldSet<uint64_t, ScriptFieldType::ULong>(field, scriptField, [&name](uint64_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U64, &data); });
							DrawScriptFieldSet<glm::vec2, ScriptFieldType::Vector2>(field, scriptField, [&name](glm::vec2& data) { return ImGui::DragFloat2(name.c_str(), glm::value_ptr(data)); });
							DrawScriptFieldSet<glm::vec3, ScriptFieldType::Vector3>(field, scriptField, [&name](glm::vec3& data) { return ImGui::DragFloat3(name.c_str(), glm::value_ptr(data)); });
							DrawScriptFieldSet<glm::vec4, ScriptFieldType::Vector4>(field, scriptField, [&name](glm::vec4& data) { return ImGui::DragFloat4(name.c_str(), glm::value_ptr(data)); });
						}
						else
						{
							ScriptFieldInstance& fieldInstance = entityFields[name];

							// Display control to set it
							DrawScriptField<float, ScriptFieldType::Float>(field, fieldInstance, 0.0f, [&name](float& data)  { return ImGui::DragScalar(name.c_str(), ImGuiDataType_Float, &data); });
							DrawScriptField<double, ScriptFieldType::Double>(field, fieldInstance, 0.0, [&name](double& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_Double, &data); });
							DrawScriptField<bool, ScriptFieldType::Bool>(field, fieldInstance, false, [&name](bool& data) { return ImGui::Checkbox(name.c_str(), &data); });
							DrawScriptField<wchar_t, ScriptFieldType::Char>(field, fieldInstance, ' ', [&name](wchar_t& data) { char sData[2]; std::wcstombs(sData, &data, 1); sData[1] = '\0'; bool ret = ImGui::InputText(name.c_str(), sData, 2); if (ret) std::mbstowcs(&data, sData, 1); return ret; });
							DrawScriptField<uint8_t, ScriptFieldType::Byte>(field, fieldInstance, 0, [&name](uint8_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U8, &data); });
							DrawScriptField<int16_t, ScriptFieldType::Short>(field, fieldInstance, 0, [&name](int16_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S16, &data); });
							DrawScriptField<int32_t, ScriptFieldType::Int>(field, fieldInstance, 0, [&name](int32_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S32, &data); });
							DrawScriptField<int64_t, ScriptFieldType::Long>(field, fieldInstance, 0, [&name](int64_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_S64, &data); });
							DrawScriptField<uint16_t, ScriptFieldType::UShort>(field, fieldInstance, 0, [&name](uint16_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U16, &data); });
							DrawScriptField<uint32_t, ScriptFieldType::UInt>(field, fieldInstance, 0, [&name](uint32_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U32, &data); });
							DrawScriptField<uint64_t, ScriptFieldType::ULong>(field, fieldInstance, 0, [&name](uint64_t& data) { return ImGui::DragScalar(name.c_str(), ImGuiDataType_U64, &data); });
							DrawScriptField<glm::vec2, ScriptFieldType::Vector2>(field, fieldInstance, { 0.0f, 0.0f }, [&name](glm::vec2& data) { return ImGui::DragFloat2(name.c_str(), glm::value_ptr(data)); });
							DrawScriptField<glm::vec3, ScriptFieldType::Vector3>(field, fieldInstance, { 0.0f, 0.0f, 0.0f }, [&name](glm::vec3& data) { return ImGui::DragFloat3(name.c_str(), glm::value_ptr(data)); });
							DrawScriptField<glm::vec4, ScriptFieldType::Vector4>(field, fieldInstance, { 0.0f, 0.0f, 0.0f, 0.0f }, [&name](glm::vec4& data) { return ImGui::DragFloat4(name.c_str(), glm::value_ptr(data)); });
						}
					}
				}
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		
			std::string label = "None";
			bool isTextureValid = false;

			if (component.Texture)
			{
				if (AssetManager::IsAssetHandleValid(component.Texture) && AssetManager::GetAssetType(component.Texture) == AssetType::Texture2D)
				{
					const auto& metadata = Project::GetActive()->GetEditorAssetManager()->GetMetadata(component.Texture);

					label = metadata.FilePath.filename().string();
					isTextureValid = true;
				}
				else
				{
					label = "Invalid";
				}
			}

			ImVec2 buttonLabelSize = ImGui::CalcTextSize(label.c_str());
			buttonLabelSize.x += 20.0f;
			float buttonLabelWidth = glm::max<float>(100.0f, buttonLabelSize.x);

			ImGui::Button(label.c_str(), {buttonLabelWidth, 0.0f});
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					AssetHandle handle = *(AssetHandle*)payload->Data;

					if (AssetManager::GetAssetType(handle) == AssetType::Texture2D)
					{
						component.Texture = handle;
					}
					else
					{
						EL_CORE_WARN("Wrong asset type!");
					}
				}

				ImGui::EndDragDropTarget();
			}

			if (isTextureValid)
			{
				ImGui::SameLine();

				ImVec2 xLabelSize = ImGui::CalcTextSize("X");
				float buttonSize = xLabelSize.y + ImGui::GetStyle().FramePadding.y * 2.0f;

				if (ImGui::Button("X", { buttonSize, buttonSize }))
				{
					component.Texture = 0;
				}
			}

			ImGui::SameLine();
			ImGui::Text("Texture");

			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
		});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](CircleRendererComponent& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](Rigidbody2DComponent& component)
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];

			if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i < 3; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (Rigidbody2DComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](BoxCollider2DComponent& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
			
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](CircleCollider2DComponent& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat("Radius", &component.Radius);

			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<TextComponent>("Text", entity, [](TextComponent& component)
		{
			ImGui::InputTextMultiline("Text String", &component.TextString, ImVec2(0, 0), ImGuiInputTextFlags_AllowTabInput);
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Kerning", &component.Kerning, 0.025f);
			ImGui::DragFloat("Line Spacing", &component.LineSpacing, 0.025f);
		});
	}

	template<typename T>
	inline void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

}