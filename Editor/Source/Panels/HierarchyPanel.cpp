#include "HierarchyPanel.h"

#include "Layer/EditorLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <regex>

namespace NL
{
#define RIGHT_COLUMN_WIDTH 180

	namespace Utils
	{
		static bool TreeNodeExStyle1(const void* str_id, const std::string& name, ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2.0f, 2.0f });
			bool isExpanded = ImGui::TreeNodeEx((void*)str_id, flags, name.c_str());
			ImGui::PopStyleVar();
			return isExpanded;
		}

		static bool ColorEdit3Style1(const std::string& label, float color3Width, nlm::vec3& color)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - color3Width);
			ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			std::string emitLabel = "##" + label;
			bool isModified = ImGui::ColorEdit3(emitLabel.c_str(), nlm::value_ptr(color), ImGuiColorEditFlags_NoInputs);
			ImGui::Columns(1);
			ImGui::PopItemWidth();

			return isModified;
		}

		static bool ColorEdit4Style1(const std::string& label, float color4Width, nlm::vec4& color)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - color4Width);
			ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			std::string emitLabel = "##" + label;
			bool isModified = ImGui::ColorEdit4(emitLabel.c_str(), nlm::value_ptr(color), ImGuiColorEditFlags_NoInputs);
			ImGui::Columns(1);
			ImGui::PopItemWidth();

			return isModified;
		}

		static bool DragFloatStyle1(const std::string& label, float dragFloatWidth, float& value, float speed, float min, float max, const std::string& format = "%.2f")
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - dragFloatWidth);
			ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			std::string emitLabel = "##" + label;
			bool isModified = ImGui::DragFloat(emitLabel.c_str(), &value, speed, min, max, format.c_str());
			ImGui::Columns(1);
			ImGui::PopItemWidth();

			return isModified;
		}

		static bool DragIntStyle1(const std::string& label, float dragIntWidth, int& value)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - dragIntWidth);
			ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			std::string emitLabel = "##" + label;
			bool isModified = ImGui::DragInt(emitLabel.c_str(), &value);
			ImGui::Columns(1);
			ImGui::PopItemWidth();

			return isModified;
		}

		static bool DragFloat3Style1(const std::string& label, float dragIntWidth, nlm::vec3& value)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - dragIntWidth);
			ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			std::string emitLabel = "##" + label;
			bool isModified = ImGui::DragFloat3(emitLabel.c_str(), nlm::value_ptr(value));
			ImGui::Columns(1);
			ImGui::PopItemWidth();

			return isModified;
		}

		static bool DragFloatStyle1(const std::string& label, float dragFloatWidth, float& value)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - dragFloatWidth);
			ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());

			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			std::string emitLabel = "##" + label;
			bool isModified = ImGui::DragFloat(emitLabel.c_str(), &value);
			ImGui::Columns(1);
			ImGui::PopItemWidth();

			return isModified;
		}

		static bool CheckBoxStyle1(const std::string& label, float checkBoxWidth, bool& value)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - checkBoxWidth);
			ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());

			ImGui::Text(label.c_str());
			ImGui::NextColumn();
			
			std::string emitLabel = "##" + label;
			bool isModified = ImGui::Checkbox(emitLabel.c_str(), &value);
			ImGui::Columns(1);
			ImGui::PopItemWidth();

			return isModified;
		}

		// Remember to ImGui::Columns(1); manually
		static bool ComboStyle1(const std::string& label, float comboWidth, const char* value)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - comboWidth);
			float width = ImGui::CalcItemWidth();
			ImGui::PushMultiItemsWidths(1, width);

			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			std::string emitLabel = "##" + label;
			bool isModified = ImGui::BeginCombo(emitLabel.c_str(), value);

			// NL_INFO("Combo Modified!");
			
			if (!isModified)
				ImGui::Columns(1);
			ImGui::PopItemWidth();
			return isModified;
		}

		static bool InputTextStyle1(const std::string& label, float textWidth, char* buffer, int size)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - textWidth);
			ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());

			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			std::string emitLabel = "##" + label;
			bool isModified = ImGui::InputText(emitLabel.c_str(), buffer, size);

			ImGui::Columns(1);
			ImGui::PopItemWidth();
			return isModified;
		}

	}

	HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene)
	{
		SetSceneContext(scene);
	}

	void HierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_EntitySelected = {};
	}

	void HierarchyPanel::OnImGuiRender(bool showHierarchy, bool showInspector)
	{
		if (showHierarchy)
		{
			ImGui::Begin("Hierarchy");

			if (m_Scene)
			{
				// Left-click on blank space
				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
					m_EntitySelected = {};

				// Right-click on blank space
				if (ImGui::BeginPopupContextWindow(0, 1))
				{
					if (ImGui::MenuItem("Create Empty Entity"))
					{
						m_EntitySelected = m_Scene->CreateEntity("Entity");
					}

					ImGui::EndPopup();
				}

				m_Scene->m_Registry.each(
					[&](auto handle)
					{
						Entity entity{ handle, m_Scene.get() };
						DrawEntityNode(entity);
					}
				);
			}

			ImGui::End();
		}

		if (showInspector)
		{
			ImGui::Begin("Inspector");

			if (m_EntitySelected)
			{
				DrawComponents(m_EntitySelected);
			}

			ImGui::End();
		}
	}

	void HierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_EntitySelected = entity;
	}

	void HierarchyPanel::DrawEntityNode(Entity entity)
	{
		const char* name = "Nameless Entity";
		if (entity.HasComponent<IdentityComponent>())
		{
			name = entity.GetComponent<IdentityComponent>().Name.c_str();
		}

		ImGuiTreeNodeFlags flags = ((m_EntitySelected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool isExpanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name);

		if (ImGui::IsItemClicked())
		{
			m_EntitySelected = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		/*if ((m_EntitySelected == entity && ImGui::IsKeyPressed(ImGuiKey_Delete)))
			entityDeleted = true;*/

		if (isExpanded)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Scene->DestroyEntity(entity);
			if (m_EntitySelected == entity)
				m_EntitySelected = {};
		}
	}

	void HierarchyPanel::DrawComponents(Entity entity)
	{
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 }); // ItemSpacing

		float addComponentButtonWidth = 150.0f;

		if (entity.HasComponent<IdentityComponent>())
		{
			auto& name = entity.GetComponent<IdentityComponent>().Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), name.c_str(), sizeof(buffer));
			ImGui::SetNextItemWidth(ImGui::GetWindowContentRegionWidth() - addComponentButtonWidth);
			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				name = std::string(buffer);
			}
		}

		ImGui::SameLine();

		ImGui::SetNextItemWidth(addComponentButtonWidth);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		// Inspector Add Components
		if (ImGui::BeginPopup("AddComponent"))
		{
			// DisplayAddComponentEntry<>("Camera");

			InspectorAddComponent<TransformComponent>("Transform");
			InspectorAddComponent<CameraComponent>("Camera");
			InspectorAddComponent<ModelRendererComponent>("Model Renderer");
			InspectorAddComponent<LightComponent>("Light");
			InspectorAddComponent<ScriptComponent>("Scripting");

			ImGui::EndPopup();
		}

#pragma region Draw Transform

		DrawComponent<TransformComponent>("Transform", entity, [](auto& entity, auto& component) {

		DrawVec3Control("Translation", component.Translation);
		nlm::vec3 rotation = nlm::degrees(component.Rotation);
		DrawVec3Control("Rotation", rotation);
		component.Rotation = nlm::radians(rotation);
		DrawVec3Control("Scale", component.Scale, 1.0f);
		ImGui::Dummy(ImVec2{ 0, 1 });

		});

#pragma endregion

#pragma region Draw Camera

		DrawComponent<CameraComponent>("Camera", entity, [](auto& entity, auto& component) {

		auto& camera = component.mCamera;

		const char* projectionTypeStrings[] = { "Ortho", "Persp" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
		// if (Utils::ComboStyle1("Projection", RIGHT_COLUMN_WIDTH, currentLightTypeString))
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - RIGHT_COLUMN_WIDTH);
		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::Text("Projection");
		ImGui::NextColumn();
		if (ImGui::BeginCombo("##Projection", currentProjectionTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = (currentProjectionTypeString == projectionTypeStrings[i]);
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					camera.SetProjectionType((Camera::ProjectionType)i);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::Columns(1);

		if (camera.GetProjectionType() == Camera::ProjectionType::Perspective)
		{
			float perspectiveVerticalFOV = nlm::degrees(camera.GetPerspectiveFOV());
			if (Utils::DragFloatStyle1("Vertical FOV", RIGHT_COLUMN_WIDTH, perspectiveVerticalFOV, 0.1f, 10.0f, 170.0f))
				camera.SetPerspectiveFOV(nlm::radians(perspectiveVerticalFOV));

			float perspectiveNear = camera.GetPerspectiveNear();
			if (Utils::DragFloatStyle1("Near Clip", RIGHT_COLUMN_WIDTH, perspectiveNear, 0.001f, 0.001f, 10.0f, "%.3f"))
				camera.SetPerspectiveNear(perspectiveNear);

			float perspectiveFar = camera.GetPerspectiveFar();
			if (Utils::DragFloatStyle1("Far Clip", RIGHT_COLUMN_WIDTH, perspectiveFar, 10.0f, 100.0f, 3000.0f, "%.0f"))
				camera.SetPerspectiveFar(perspectiveFar);
		}
		else
		{
			float orthoSize = camera.GetOrthographicSize();
			if (Utils::DragFloatStyle1("Size", RIGHT_COLUMN_WIDTH, orthoSize, 1.0f, 1.0f, 500.0f, "%.0f"))
				camera.SetOrthographicSize(orthoSize);

			float orthoNear = camera.GetOrthographicNear();
			if (Utils::DragFloatStyle1("Near Clip", RIGHT_COLUMN_WIDTH, orthoNear, 0.001f, 0.001f, 10.0f, "%.3f"))
				camera.SetOrthographicNear(orthoNear);

			float orthoFar = camera.GetOrthographicFar();
			if (Utils::DragFloatStyle1("Far Clip", RIGHT_COLUMN_WIDTH, orthoFar, 10.0f, 100.0f, 3000.0f, "%.0f"))
				camera.SetOrthographicFar(orthoFar);
		}

		if (Utils::CheckBoxStyle1("Fixed Aspect Ratio", RIGHT_COLUMN_WIDTH, component.FixedAspectRatio))
		{
			// m_RuntimeCameraUpdateCallback();
		}

		if (component.FixedAspectRatio)
		{
			float width = camera.GetViewportWidth();
			float height = camera.GetViewportHeight();

			if (Utils::DragFloatStyle1("Width", RIGHT_COLUMN_WIDTH, width, 10.0f, 50.0f, 1920.0f, "%.0f"))
			{
				camera.SetViewportWidth(width);
				// m_RuntimeCameraUpdateCallback();
			}

			if (Utils::DragFloatStyle1("Height", RIGHT_COLUMN_WIDTH, height, 10.0f, 50.0f, 1080.0f, "%.0f"))
			{
				camera.SetViewportHeight(height);
				// m_RuntimeCameraUpdateCallback();
			}
		}

		// Clear Flags
		const char* clearFlagStrings[] = { "Color", "Skybox" };
		const char* currentClearFlagStrings = clearFlagStrings[(int)camera.GetClearFlagType()];
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - RIGHT_COLUMN_WIDTH);
		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::Text("Clear Flags");
		ImGui::NextColumn();
		if (ImGui::BeginCombo("##ClearFlags", currentClearFlagStrings))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = (currentClearFlagStrings == clearFlagStrings[i]);
				if (ImGui::Selectable(clearFlagStrings[i], isSelected))
				{
					currentClearFlagStrings = clearFlagStrings[i];
					camera.SetClearFlagType((Camera::ClearFlagType)i);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::Columns(1);

		if (camera.GetClearFlagType() == Camera::ClearFlagType::Color)
		{
			/*if (ImGui::TreeNode("Clear Color"))
			{
				ImGui::ColorEdit4("##", nlm::value_ptr(component.ClearColor), ImGuiColorEditFlags_NoInputs);
				ImGui::TreePop();
			}*/
			Utils::ColorEdit4Style1("Clear Color", RIGHT_COLUMN_WIDTH, component.ClearColor);
		}

		});

#pragma endregion
		
#pragma region Draw Model Renderer

		DrawComponent<ModelRendererComponent>("Model Renderer", entity, [](auto& entity, auto& component) {

		const auto& shaderNameMap = Library<Shader>::GetInstance().GetShaderNameMap();
		// NL_ENGINE_TRACE("Default shader name: {0}", Library<Shader>::GetInstance().GetDefaultShaderName());
		
		Ref<Model> model = component.mModel;

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - 420.0f);
		ImGui::Text("Model Path");
		ImGui::NextColumn();

		std::string modelPath = component.Path;
		// NL_ENGINE_INFO("Draw Model Renderer Component modelPath: {0}", modelPath);
		ImGui::Text(std::string_view(modelPath.c_str() + modelPath.find("Assets")).data());

		ImGui::SameLine();
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushID("ImportModel");
		if (ImGui::Button("+", buttonSize))
		{
			std::string filepath = Application::GetInstance().OpenFileDialogue(L"Model(*.obj;*.fbx;*.dae;*.gltf)\0*.obj;*.fbx;*.dae;*.gltf\0\0");
			size_t pos = filepath.find("Models");
			std::filesystem::path path;
			if (pos != std::string::npos)
			{
				filepath = filepath.substr(pos);
				path = PathConfig::GetInstance().GetAssetsFolder() / filepath.c_str();
				NL_ENGINE_INFO("New model path: {0}", path);
			}
			else if (!filepath.empty())
			{
				NL_ENGINE_ASSERT(false, "Only support models from Models Folder!");
			}

			if (!filepath.empty())
			{
				if (component.mModel)
					component.mModel->DeleteMeshesAndTexturesReference();
				component = ModelRendererComponent(path.string());
			}
		}
		ImGui::PopID();

		ImGui::Columns(1);

		model = component.mModel;
		if (model == nullptr)
			return;

		if (Utils::TreeNodeExStyle1((void*)"Materials", "Materials"))
		{
			for (auto& item : model->GetMaterials())
			{
				std::string matName = item.first;
				Ref<Material> mat = item.second;

				ImGui::PushID(&mat);

				std::string shaderName = mat->GetShaderName();				
				if (ImGui::TreeNode(matName.c_str()))
				{
					// Select Shaders
					if (ImGui::BeginCombo("Shader", shaderName.c_str()))
					{
						Library<Shader>::GetInstance().TraverseShadersFolder();
						const auto& shaderNameMap = Library<Shader>::GetInstance().GetShaderNameMap();

						if (!shaderNameMap.contains(shaderName))
							shaderName = Library<Shader>::GetInstance().GetDefaultShaderName();

						for (const auto& pair : shaderNameMap)
						{
							std::string name = pair.first;
							bool isSelected = name == shaderName;
							if (ImGui::Selectable(name.c_str(), isSelected))
							{
								if (!isSelected)
								{
									shaderName = name;
									mat->LoadShader(shaderName);
									mat->UpdateSampler2DinProperties();
								}
							}
						}
						
						ImGui::EndCombo();
					}

					// Exposed Shader Properties
					DrawShaderProperties(mat);

					ImGui::TreePop();
				}

				ImGui::PopID();
			}

			ImGui::TreePop();
		}

		// Bones
		if (model->HasBones())
		{
			bool open = Utils::TreeNodeExStyle1((void*)"Bones", "Bones");

			std::pair<int, int> adjust = { -1, -1 };

			if (ImGui::BeginDragDropTarget()) {

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BONE_INFO"))
				{
					adjust.first = *((int*)payload->Data);
					adjust.second = -1;
				}

				ImGui::EndDragDropTarget();
			}
			
			if (open)
			{
				DrawBonesHierarchy(model, adjust);
				ImGui::TreePop();
			}

			auto& bones = model->GetBonesNotConst();
			if (adjust.first != -1)
			{
				if (adjust.second == -1)
				{
					auto& srcBone = bones[adjust.first];
					if (srcBone.parentID != -1)
						bones[srcBone.parentID].Childrens.erase(srcBone.ID);
					srcBone.parentID = -1;
				}
				else
				{
					auto& srcBone = bones[adjust.first];
					auto& dstBone = bones[adjust.second];
					
					int fa = dstBone.parentID;
					bool flag = true;
					while (fa != -1)
					{
						if (fa == srcBone.ID)
						{
							flag = false;
							break;
						}
						fa = bones[fa].parentID;
					}

					if (flag)
					{
						if (srcBone.parentID != -1)
							bones[srcBone.parentID].Childrens.erase(srcBone.ID);
						srcBone.parentID = dstBone.ID;
						dstBone.Childrens.insert(srcBone.ID);
					}
				}

				model->CalculateFinalBoneMatrices();
			}
		}

		});

#pragma endregion

#pragma region Draw Lighting

		DrawComponent<LightComponent>("Light", entity, [scene = m_Scene](auto& entity, auto& component) {

		// Light Type Begin
		const char* lightTypeStrings[] = { "Dir", "Point" };
		const char* currentLightTypeString = lightTypeStrings[(int)component.Type];
		// if (Utils::ComboStyle1("Projection", RIGHT_COLUMN_WIDTH, currentLightTypeString))
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - RIGHT_COLUMN_WIDTH);
		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::Text("Type");
		ImGui::NextColumn();
		if (ImGui::BeginCombo("##LightType", currentLightTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = (currentLightTypeString == lightTypeStrings[i]);
				if (ImGui::Selectable(lightTypeStrings[i], isSelected))
				{
					currentLightTypeString = lightTypeStrings[i];
					component.Type = (LightType)i;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		// Light Type End

		float& intensity = component.Intensity;
		Utils::DragFloatStyle1("Intensity", RIGHT_COLUMN_WIDTH, intensity, 0.01f, 0.0f, 10.0f, "%.2f");

		nlm::vec3& color = component.Color;
		Utils::ColorEdit3Style1("Color", RIGHT_COLUMN_WIDTH, color);

		});

#pragma endregion

#pragma region Draw Scripting

		DrawComponent<ScriptComponent>("Scripting", entity, [scene = m_Scene](auto& entity, auto& component) {

		// component.ScriptClassExists = ScriptEngine::GetInstance().EntityClassExists(component.ClassName);

		static char buffer[64];
		strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

		if (!component.ScriptClassExists)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

		if (Utils::InputTextStyle1("Class", RIGHT_COLUMN_WIDTH, buffer, sizeof(buffer)))
		{
			component.ClassName = buffer;

			if (!component.ScriptClassExists)
				ImGui::PopStyleColor();

			NL_ENGINE_TRACE("Entity Script Class Modified");

			return;
		}

		Ref<ScriptInstance> instance = ScriptEngine::GetInstance().GetScriptInstance(entity.GetID());

		NL_ASSERT(scene != nullptr, "");

		// If this is Runtime Scene and not pausing
		if (scene->IsPlaying() && instance)
		{
			const auto& fields = instance->GetScriptClass()->GetFields();
			for (const auto& [name, field] : fields)
			{
				// Float
				if (field.Type == ScriptFieldType::Float)
				{
					float data = instance->GetFieldValue<float>(name);
					if (Utils::DragFloatStyle1(name, RIGHT_COLUMN_WIDTH, data))
					{
						instance->SetFieldValue(name, data);
					}
				}
				// Int
				else if (field.Type == ScriptFieldType::Int)
				{
					int data = instance->GetFieldValue<int>(name);
					if (Utils::DragIntStyle1(name, RIGHT_COLUMN_WIDTH, data))
					{
						instance->SetFieldValue(name, data);
					}
				}
				// Vector3
				else if (field.Type == ScriptFieldType::Vector3)
				{
					nlm::vec3 data = instance->GetFieldValue<nlm::vec3>(name);
					if (DrawVec3Control(name, data))
					{
						instance->SetFieldValue(name, data);
					}
				}
			}
		}
		// Editor
		else if (scene->IsEditor() && instance)
		{
			Ref<ScriptClass> entityClass = ScriptEngine::GetInstance().GetEntityClass(component.ClassName);
			const auto& fields = entityClass->GetFields();

			auto& fieldInstances = ScriptEngine::GetInstance().GetScriptFieldInstances(entity);
			for (const auto& [name, field] : fields)
			{
				bool flag = fieldInstances.find(name) == fieldInstances.end();

				// Float
				if (field.Type == ScriptFieldType::Float)
				{
					float data = flag ? instance->GetFieldValue<float>(name) : fieldInstances[name].GetValue<float>();
					if (Utils::DragFloatStyle1(name, RIGHT_COLUMN_WIDTH, data))
					{
						ScriptFieldInstance& fieldInstance = fieldInstances[name];
						fieldInstance.Field = field;
						fieldInstance.SetValue(data);
						instance->SetFieldValue(name, data);
					}
				}
				// Int
				else if (field.Type == ScriptFieldType::Int)
				{
					int data = flag ? instance->GetFieldValue<int>(name) : fieldInstances[name].GetValue<int>();
					if (Utils::DragIntStyle1(name, RIGHT_COLUMN_WIDTH, data))
					{
						ScriptFieldInstance& fieldInstance = fieldInstances[name];
						fieldInstance.Field = field;
						fieldInstance.SetValue(data);
						instance->SetFieldValue(name, data);
					}
				}
				// Vector3
				else if (field.Type == ScriptFieldType::Vector3)
				{
					nlm::vec3 data = flag ? instance->GetFieldValue<nlm::vec3>(name) : fieldInstances[name].GetValue<nlm::vec3>();
					if (DrawVec3Control(name, data))
					{
						ScriptFieldInstance& fieldInstance = fieldInstances[name];
						fieldInstance.Field = field;
						fieldInstance.SetValue(data);
						instance->SetFieldValue(name, data);
					}
				}
			}
		}

		if (!component.ScriptClassExists)
			ImGui::PopStyleColor();

		});

#pragma endregion

		// ImGui::PopStyleVar(); // ItemSpacing

	}

	bool HierarchyPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		bool valueChanged = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - columnWidth * 3);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// NL_ENGINE_INFO("CalcItemWidth: {0}", ImGui::CalcItemWidth());
		float width = ImGui::CalcItemWidth();
		ImGui::PushMultiItemsWidths(3, width);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			valueChanged = true;
			values.x = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChanged |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			valueChanged = true;
			values.y = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChanged |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			valueChanged = true;
			values.z = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChanged |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return valueChanged;
	}

	void HierarchyPanel::DrawShaderProperties(Ref<Material> mat)
	{
		for (ShaderProperty& prop : mat->GetShaderPropertiesNotConst())
		{
			switch (prop.Type)
			{
			case ShaderUniformType::Color3:
				nlm::vec3 color = std::get<nlm::vec3>(prop.Value);
				if (ImGui::ColorEdit3(prop.Name.c_str(), nlm::value_ptr(color)))
				{
					prop.Value = color;
				}
				break;
			case ShaderUniformType::Sampler2D:

				ImGui::PushID(&prop);
				if (ImGui::TreeNode(prop.Name.c_str()))
				{
					const std::string& oldTexPath = std::get<std::string>(prop.Value);
					Ref<Texture2D> oldTex = Library<Texture2D>::GetInstance().Fetch(oldTexPath);

					if (ImGui::ImageButton((ImTextureID)oldTex->GetRendererID(), ImVec2(64, 64), ImVec2(0, 0), ImVec2(1, 1), 0))
					{
						std::string filepath = Application::GetInstance().OpenFileDialogue(L"Texture2D(*.png)\0*.png\0\0");

						size_t pos = filepath.find("Assets");
						if (pos != std::string::npos)
						{
							filepath = PathConfig::GetInstance().GetAssetsFolder().string() + filepath.substr(pos + 6);
							Ref<Texture2D> newTex = Library<Texture2D>::GetInstance().Fetch(filepath);

							/*if (Library<Texture2D>::GetInstance().Contains(filepath))
							{
								newTex = Library<Texture2D>::GetInstance().Get(filepath);
							}
							else
							{
								newTex = Texture2D::Create(filepath);
								Library<Texture2D>::GetInstance().Add(filepath, newTex);
							}*/

							oldTex.reset();
							mat->ReplaceTexture(prop.Name, newTex);

							prop.Value = filepath;

						}
						else if (!filepath.empty())
						{
							NL_ENGINE_ASSERT(false, "Only support textures from Textures Folder!");
						}
					}

					ImGui::TreePop();
				}
				ImGui::PopID();
				break;

			default:
				break;
			}
		}
	}

	void HierarchyPanel::DrawBonesHierarchy(Ref<Model> model, std::pair<int, int>& adjust)
	{
		auto& bones = model->GetBonesNotConst();
		auto& boneMap = model->GetBoneMap();
		for (auto& pair : bones)
		{
			auto& bone = pair.second;
			if (bone.parentID == -1)	// Root
			{
				// ImGui::SetNextItemOpen(true);
				std::string name = fmt::format("{}({})", bone.Name, bone.ID);

				ImGui::PushID(&bone);
				bool open = ImGui::TreeNode(name.c_str());

				if (ImGui::BeginDragDropTarget()) {
					
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BONE_INFO"))
					{
						adjust.first = *((int*)payload->Data);
						adjust.second = bone.ID;
					}

					ImGui::EndDragDropTarget();
				}

				if (ImGui::BeginDragDropSource()) {
					
					ImGui::SetDragDropPayload("BONE_INFO", &(bone.ID), sizeof(&(bone.ID)));
					ImGui::EndDragDropSource();
				}

				if (open)
				{
					for (auto& childID : bone.Childrens)
					{
						NL_ASSERT(childID < bones.size(), "");
						DrawBonesRecursive(bones[childID], bones, adjust);
					}
					
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
		}
	}

	void HierarchyPanel::DrawBonesRecursive(BoneInfo& bone, std::map<int, BoneInfo>& bones, std::pair<int, int>& adjust)
	{
		// ImGui::SetNextItemOpen(true);
		std::string name = fmt::format("{}({})", bone.Name, bone.ID);
		ImGui::PushID(&bone);
		bool open = ImGui::TreeNode(name.c_str());

		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BONE_INFO"))
			{
				adjust.first = *((int*)payload->Data);
				adjust.second = bone.ID;
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginDragDropSource()) {

			ImGui::SetDragDropPayload("BONE_INFO", &(bone.ID), sizeof(&(bone.ID)));
			ImGui::EndDragDropSource();
		}

		if (open)
		{
			// NL_ENGINE_INFO("Open Tree Node, {0}", name);
			for (auto& childID : bone.Childrens)
			{
				NL_ASSERT(childID < bones.size(), "");
				DrawBonesRecursive(bones[childID], bones, adjust);
			}

			ImGui::TreePop();
		}
		ImGui::PopID();
	}

	template<Component C>
	void HierarchyPanel::InspectorAddComponent(const char* componentName)
	{
		if (!m_EntitySelected.HasComponent<C>())
		{
			if (ImGui::MenuItem(componentName))
			{
				m_EntitySelected.AddComponent<C>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	template<Component C, typename UIFunction>
	void HierarchyPanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<C>())
		{
			auto& component = entity.GetComponent<C>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 });
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			//ImGui::Dummy(ImVec2{ 0.0f, 1.0f });
			ImGui::Separator();
			bool isExpanded = ImGui::TreeNodeEx((void*)typeid(C).hash_code(), treeNodeFlags, name.c_str());
			
			ImGui::PopStyleVar(2);

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
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

			if (isExpanded)
			{
				uiFunction(entity, component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<C>();

		}
	}

}