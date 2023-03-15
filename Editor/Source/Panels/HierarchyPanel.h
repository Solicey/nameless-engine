#pragma once

#include <Engine.h>

namespace NL
{
	class HierarchyPanel
	{
	public:
		HierarchyPanel() = default;
		HierarchyPanel(const Ref<Scene>& scene);

		void SetCurrentScene(const Ref<Scene>& scene);

		void OnImGuiRender(bool showHierarchy, bool showInspector);

		void SetSelectedEntity(Entity entity);

		Entity GetSelectedEntity() const { return m_EntitySelected; }

	private:
		void DrawEntityNode(Entity entity);
		

	private:
		Ref<Scene> m_Scene;
		Entity m_EntitySelected;
	};
}