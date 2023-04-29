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
		void UpdateParticleSystem(const nlm::mat4& viewMat, const nlm::mat4& projMat);

	private:
		// Ref<Shader> m_GrayScaleShader;
		Ref<TextureCubeMap> m_SkyboxTextureCubemap;
		Ref<Shader> m_SkyboxShader;
		Ref<Model> m_Skybox;

		Ref<Shader> m_GizmosShader;
		Ref<Model> m_Gizmos;
		Ref<Texture2D> m_PointGizmosTexture, m_DirGizmosTexture;

		// Time
		float m_Duration = 0.0f;
		float m_DeltaTime = 0.0f;
	};
}