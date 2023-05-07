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
		Ref<Texture2D> m_PointGizmosTexture, m_DirGizmosTexture;

		Ref<Shader> m_SpriteShader;

		// Time
		float m_TotalTime = 0.0f;
		float m_DeltaTime = 0.0f;
	};
}