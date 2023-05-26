#pragma once

#include "ECS/System/System.h"
#include "Resources/Shader.h"
#include "ECS/Entity/Entity.h"

namespace NL
{
	class RenderSystem : public System
	{
	public:
		RenderSystem(Scene* scene);

		virtual void OnStartRuntime() override;
		virtual void OnStopRuntime(Scene* editorScene) override;
		virtual void OnUpdateRuntime(TimeStep ts) override;

		virtual void OnStartEditor() override;
		virtual void OnUpdateEditor(TimeStep ts) override;

	private:
		void UpdateParticleSystem();
		void DrawModel(const Ref<Model>& model, const nlm::mat4& transform, const nlm::mat4& viewMatrix, int entityId = -1, const Ref<Shader>& shader = nullptr);
		void DrawSprite(const Ref<Shader>& shader,
			const Ref<Texture2D>& texture,
			const nlm::mat4& transform,
			const nlm::vec4& color,
			SpriteCameraReaction camReact,
			int entityId);

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