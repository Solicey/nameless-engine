#include "HierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace NL
{
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
						m_EntitySelected = m_Scene->CreateEntity("Empty Entity");
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

			InspectorAddComponent<ModelRendererComponent>("Model Renderer");

			ImGui::EndPopup();
		}

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			
		DrawVec3Control("Translation", component.Translation);
		nlm::vec3 rotation = nlm::degrees(component.Rotation);
		DrawVec3Control("Rotation", rotation);
		component.Rotation = nlm::radians(rotation);
		DrawVec3Control("Scale", component.Scale, 1.0f);
		
		});	

		DrawComponent<ModelRendererComponent>("Model Renderer", entity, [](auto& component) {
			
		const auto& shaderNameMap = Library<Shader>::GetInstance().GetShaderNameMap();
		// NL_ENGINE_TRACE("Default shader name: {0}", Library<Shader>::GetInstance().GetDefaultShader());
		


		});

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

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool isExpanded = ImGui::TreeNodeEx((void*)typeid(C).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

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
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<C>();
		}
	}

}