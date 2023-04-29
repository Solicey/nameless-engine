#pragma once

#include "ECS/System/System.h"
#include "Resources/Shader.h"

namespace NL
{
	class RenderSystem : public System
	{
	public:
		RenderSystem(Scene* scene);

		virtual void OnStartRuntime() override;
		virtual void OnStopRuntime(Scene* editorScene) override;
		virtual void OnUpdateRuntime(TimeStep ts, Entity cameraEntity) override;

		virtual void OnStartEditor() override;
		virtual void OnUpdateEditor(TimeStep ts, EditorCamera& camera, Entity selectedEntity) override;

	private:
		void UpdateParticleSystem();

	private:
		// Ref<Shader> m_GrayScaleShader;
		Ref<TextureCubeMap> m_SkyboxTextureCubemap;
		Ref<Shader> m_SkyboxShader;
		Ref<Model> m_Skybox;

		Ref<Shader> m_GizmosShader;
		Ref<Model> m_Gizmos;
		Ref<Texture2D> m_PointGizmosTexture, m_DirGizmosTexture;

		float m_DeltaTime;
		float m_TotalTime;
		int m_Input = 0;	// Input
		int m_Output = 1;	// Output
	};
}