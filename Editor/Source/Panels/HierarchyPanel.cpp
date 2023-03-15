#include "HierarchyPanel.h"

#include <imgui.h>

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
						m_Scene->CreateEntity("Empty Entity");

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

		bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name);

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

		if ((m_EntitySelected == entity && ImGui::IsKeyPressed(ImGuiKey_Delete)))
			entityDeleted = true;

		if (isOpened)
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

}