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
		void DrawComponents(Entity entity);
		
		template<Component C>
		void InspectorAddComponent(const char* componentName);

		template<Component C, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction);

		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);

		static void DrawShaderProperties(Ref<Material> mat);

	private:
		Ref<Scene> m_Scene;
		Entity m_EntitySelected;
	};
}