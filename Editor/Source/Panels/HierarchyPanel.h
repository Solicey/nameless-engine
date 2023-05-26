#pragma once

#include <Engine.h>

namespace NL
{
	class HierarchyPanel
	{
		typedef std::function<void()> Callback;

	public:
		HierarchyPanel() = default;
		HierarchyPanel(const Ref<Scene>& scene);
		void SetUpdateRuntimeCameraCallback(Callback callback)
		{
			m_RuntimeCameraUpdateCallback = std::move(callback);
		}

		void SetSceneContext(const Ref<Scene>& scene);

		void OnImGuiRender(bool showHierarchy, bool showInspector);

		void SetSelectedEntity(Entity entity);

		Entity GetSelectedEntity() const { return m_EntitySelected; }

		// entt entity id
		int GetSelectedEntityId() const
		{
			if (m_EntitySelected)
				return (int)(uint32_t)(m_EntitySelected);
			return -1;
		}

		void DrawEditorCameraPostProcessShaderCombo(Ref<Material>& mat);

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		
		template<Component C>
		void InspectorAddComponent(const char* componentName);

		template<Component C, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool canDelete = true);

		static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 140.0f);

		static void DrawShaderProperties(Ref<Material> mat);
		static void DrawBonesHierarchy(Ref<Model> model, std::pair<int, int>& adjust);
		static void DrawBonesRecursive(BoneInfo& boneInfo, std::map<int, BoneInfo>& bones, std::pair<int, int>& adjust);

		template<Component C, typename UIFunction>
		static void DrawShaderCombo(Ref<Material>& mat, const std::string& matName, bool& shaderSelectClick, UIFunction uiFunction, ShaderUse shaderUse, const Ref<Scene>& scene, bool hasTreeNode = true);

	private:
		Ref<Scene> m_Scene;
		Entity m_EntitySelected;
		Callback m_RuntimeCameraUpdateCallback;
		bool m_ShaderSelectOpen = false;
	};
}