#include "HierarchyPanel.h"

#include "Layer/EditorLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <regex>

namespace NL
{
	namespace Utils
	{
		static bool TreeNodeExStyle1(const void* str_id, const std::string& name, ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2.0f, 2.0f });
			bool isExpanded = ImGui::TreeNodeEx((void*)str_id, flags, name.c_str());
			ImGui::PopStyleVar();
			return isExpanded;
		}

	}

	HierarchyPanel::HierarchyPanel(const Ref<Scene>& scene)
	{
		SetCurrentScene(scene);
	}

	void HierarchyPanel::SetCurrentScene(const Ref<Scene>& scene)
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

		if (entity.HasComponent<IdentityComponent>())
		{
			auto& name = entity.GetComponent<IdentityComponent>().Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), name.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				name = std::string(buffer);
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		// Inspector Add Components
		if (ImGui::BeginPopup("AddComponent"))
		{
			// DisplayAddComponentEntry<>("Camera");

			InspectorAddComponent<TransformComponent>("Transform");
			InspectorAddComponent<CameraComponent>("Camera");
			InspectorAddComponent<ModelRendererComponent>("Model Renderer");
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

		const char* projectionTypeStrings[] = { "Orthographic", "Perspective" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
		if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = (currentProjectionTypeString == projectionTypeStrings[i]);
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					camera.SetProjectionType((Camera::ProjectionType)i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (camera.GetProjectionType() == Camera::ProjectionType::Perspective)
		{
			float perspectiveVerticalFOV = nlm::degrees(camera.GetPerspectiveFOV());
			if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFOV))
				camera.SetPerspectiveFOV(nlm::radians(perspectiveVerticalFOV));

			float perspectiveNear = camera.GetPerspectiveNear();
			if (ImGui::DragFloat("Near Clip", &perspectiveNear))
				camera.SetPerspectiveNear(perspectiveNear);

			float perspectiveFar = camera.GetPerspectiveFar();
			if (ImGui::DragFloat("Far Clip", &perspectiveFar))
				camera.SetPerspectiveFar(perspectiveFar);
		}
		else
		{
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);

			float orthoNear = camera.GetOrthographicNear();
			if (ImGui::DragFloat("Near Clip", &orthoNear))
				camera.SetOrthographicNear(orthoNear);

			float orthoFar = camera.GetOrthographicFar();
			if (ImGui::DragFloat("Far Clip", &orthoFar))
				camera.SetOrthographicFar(orthoFar);
		}

		if (ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio))
		{
			// m_RuntimeCameraUpdateCallback();
		}

		if (component.FixedAspectRatio)
		{
			float width = camera.GetViewportWidth();
			float height = camera.GetViewportHeight();

			if (ImGui::DragFloat("Width", &width))
			{
				camera.SetViewportWidth(width);
				// m_RuntimeCameraUpdateCallback();
			}

			if (ImGui::DragFloat("Height", &height))
			{
				camera.SetViewportHeight(height);
				// m_RuntimeCameraUpdateCallback();
			}
		}

		});

#pragma endregion
		
#pragma region Draw Mesh Renderer

		DrawComponent<ModelRendererComponent>("Model Renderer", entity, [](auto& entity, auto& component) {
			
		const auto& shaderNameMap = Library<Shader>::GetInstance().GetShaderNameMap();
		// NL_ENGINE_TRACE("Default shader name: {0}", Library<Shader>::GetInstance().GetDefaultShaderName());
		
		Ref<Model> model = component.mModel;

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 120.0f);
		ImGui::Text("Model Path");
		ImGui::NextColumn();

		std::string modelPath = component.Path;
		// NL_ENGINE_INFO("Draw Model Renderer Component modelPath: {0}", modelPath);
		ImGui::Text(std::string_view(modelPath.c_str() + modelPath.find("Assets")).data());

		ImGui::SameLine();
		if (ImGui::Button("..."))
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
				component = ModelRendererComponent(path.string(), (uint32_t)entity, component.Flags);
			}
		}

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

				ImGui::PushID(matName.c_str());

				std::string shaderName = mat->GetShaderName();				

				if (ImGui::TreeNode((void*)matName.c_str(), matName.c_str()))
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

		});

#pragma endregion

#pragma region Draw Scripting

		DrawComponent<ScriptComponent>("Scripting", entity, [scene = m_Scene](auto& entity, auto& component) {

		bool scriptClassExists = ScriptEngine::GetInstance().EntityClassExists(component.ClassName);

		static char buffer[64];
		strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

		if (!scriptClassExists)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

		if (ImGui::InputText("Class", buffer, sizeof(buffer)))
		{
			component.ClassName = buffer;

			if (!scriptClassExists)
				ImGui::PopStyleColor();

			return;
		}

		// If this is Runtime Scene and not pausing
		if (scene != nullptr && scene->IsRunning())
		{
			Ref<ScriptInstance> instance = ScriptEngine::GetInstance().GetScriptInstance(entity.GetID());
			if (instance)
			{
				const auto& fields = instance->GetScriptClass()->GetFields();
				for (const auto& [name, field] : fields)
				{
					if (field.Type == ScriptFieldType::Float)
					{
						float data = instance->GetFieldValue<float>(name);
						if (ImGui::DragFloat(name.c_str(), &data))
						{
							instance->SetFieldValue(name, data);
						}
					}
				}
			}
		}
		else
		{
			if (scriptClassExists)
			{
				Ref<ScriptClass> entityClass = ScriptEngine::GetInstance().GetEntityClass(component.ClassName);
				const auto& fields = entityClass->GetFields();

				auto& fieldInstances = ScriptEngine::GetInstance().GetScriptFieldInstances(entity);
				for (const auto& [name, field] : fields)
				{
					// Field has been set in editor
					if (fieldInstances.find(name) != fieldInstances.end())
					{
						ScriptFieldInstance& fieldInstance = fieldInstances.at(name);

						if (field.Type == ScriptFieldType::Float)
						{
							float data = fieldInstance.GetValue<float>();
							if (ImGui::DragFloat(name.c_str(), &data))
								fieldInstance.SetValue(data);
						}
					}
					else
					{
						if (field.Type == ScriptFieldType::Float)
						{
							float data = 0.0f;
							if (ImGui::DragFloat(name.c_str(), &data))
							{
								ScriptFieldInstance& fieldInstance = fieldInstances[name];
								fieldInstance.Field = field;
								fieldInstance.SetValue(data);
							}
						}
					}
				}
			}
		}

		if (!scriptClassExists)
			ImGui::PopStyleColor();

		});

#pragma endregion

		// ImGui::PopStyleVar(); // ItemSpacing

	}

	void HierarchyPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
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

			default:
				break;
			}
		}
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

			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 4 });
			//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Dummy(ImVec2{ 0.0f, 1.0f });
			ImGui::Separator();
			bool isExpanded = ImGui::TreeNodeEx((void*)typeid(C).hash_code(), treeNodeFlags, name.c_str());
			
			//ImGui::PopStyleVar(2);

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
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