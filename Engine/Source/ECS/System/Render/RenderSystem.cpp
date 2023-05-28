#include "nlpch.h"

#include "RenderSystem.h"

#include "Core/Misc/PathConfig.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"
#include "Renderer/Renderer.h"
#include "Resources/Libraries/ShaderLibrary.h"

namespace NL
{
	RenderSystem::RenderSystem(Scene* scene)
		: System(scene)
	{
		m_SkyboxShader = Library<Shader>::GetInstance().Fetch("Skybox.glsl");
		NL_ENGINE_ASSERT(m_SkyboxShader, "Skybox shader does NOT exists!");

		std::string modelsFolder = PathConfig::GetInstance().GetModelsFolder().string();
		m_Skybox = ModelLoader::Create(modelsFolder + "/DontModify/Skybox.obj", ModelLoaderFlags::Triangulate);
		m_SkyboxTextureCubemap = Library<TextureCubeMap>::GetInstance().FetchDefault();

		m_GizmosShader = Library<Shader>::GetInstance().Fetch("Gizmos.glsl");

		std::string assetFolder = PathConfig::GetInstance().GetAssetsFolder().string();
		m_PointGizmosTexture = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Icons/PointLight.png");
		m_DirGizmosTexture = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Icons/DirLight.png");

		m_SpriteShader = Library<Shader>::GetInstance().Fetch("Sprite.glsl");
	}

	void RenderSystem::OnStartRuntime()
	{
		m_TotalTime = m_DeltaTime = 0;
	}

	void RenderSystem::OnStopRuntime(Scene* editorScene)
	{
		m_TotalTime = m_DeltaTime = 0;
	}

	void RenderSystem::OnUpdateRuntime(TimeStep ts)
	{
		m_TotalTime += ts;
		m_DeltaTime = ts;

		auto settings = m_Scene->GetSettingsEntity();
		auto cameraEntity = m_Scene->GetEntityWithID(settings.GetComponent<SettingsComponent>().RuntimeCameraID);

		//NL_ENGINE_INFO("Runtime Camera ID: {0}", settings.GetComponent<SettingsComponent>().RuntimeCameraID);

		if (cameraEntity == Entity())
			return;

		if (!cameraEntity.HasComponent<CameraComponent>())
			return;

		if (!cameraEntity.HasComponent<TransformComponent>())
		{
			NL_ENGINE_ASSERT(false, "Entity with CameraComponent should also have TransformComponent!");
			return;
		}

		auto& camera = cameraEntity.GetComponent<CameraComponent>();
		auto& camTransform = cameraEntity.GetComponent<TransformComponent>();
		auto clearFlag = camera._Camera->GetClearFlagType();
		auto viewMatrix = nlm::inverse(camTransform.GetTransform());

		m_Scene->PackUpLightDatas();
		Renderer::SetUniformBuffer(camera._Camera, camTransform.GetTransform(), camTransform.GetTranslation());

		// Render Models (Opaque)
		{
			auto view = m_Scene->Registry.view<TransformComponent, ModelRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& model = entity.GetComponent<ModelRendererComponent>();

				if (model._Model != nullptr)
				{
					DrawModel(model._Model, transform.GetTransform(), viewMatrix, (int)(uint32_t)entity);
				}
			}
		}

		// Render Sprites
		{
			// Renderer::SetCullFace(CullFace::Front);
			auto view = m_Scene->Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteRendererComponent>();

				if (sprite.SpriteTexture != nullptr)
				{
					DrawSprite(m_SpriteShader, sprite.SpriteTexture, transform.GetTransform(), sprite.Color, sprite.Reaction, (int)(uint32_t)entity);
				}
			}
			// Renderer::SetCullFace(CullFace::Back);
		}

		if (clearFlag == Camera::ClearFlagType::Skybox)
		{
			//NL_ENGINE_INFO("Skybox!");
			m_SkyboxShader->Bind();

			Renderer::DepthFunc(DepthComp::Lequal);
			Renderer::SetCullFace(CullFace::Front);

			// tmp
			m_SkyboxTextureCubemap->Bind(0);
			m_SkyboxShader->SetUniformInt("u_Skybox", 0);
			NL_ENGINE_ASSERT(cameraEntity.HasComponent<TransformComponent>(), "Camera does NOT have transform!");
			DrawModel(m_Skybox, nlm::translate(nlm::mat4(1.0f), cameraEntity.GetComponent<TransformComponent>().GetTranslation()), viewMatrix, -1, m_SkyboxShader);

			Renderer::DepthFunc(DepthComp::Less);
			Renderer::SetCullFace(CullFace::Back);
		}

		UpdateParticleSystem();

	}

	void RenderSystem::OnStartEditor()
	{
		m_TotalTime = m_DeltaTime = 0;
	}

	void RenderSystem::OnUpdateEditor(TimeStep ts)
	{
		m_TotalTime += ts;
		m_DeltaTime = ts;

		auto settings = m_Scene->GetSettingsEntity();
		auto &camera = settings.GetComponent<SettingsComponent>().EditorCamera;
		bool renderGizmos = settings.GetComponent<SettingsComponent>().ShowGizmos;
		nlm::mat4 cameraRotation = nlm::mat4(camera->GetOrientation());
		bool isDeferred = settings.GetComponent<SettingsComponent>().RenderMode == RenderMode::Deferred;
		const auto& viewMatrix = camera->GetViewMatrix();

		m_Scene->PackUpLightDatas();
		Renderer::SetUniformBuffer(camera);

		// Render Entities (Opaque)
		{
			auto view = m_Scene->Registry.view<TransformComponent, ModelRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& model = entity.GetComponent<ModelRendererComponent>();

				if (model._Model != nullptr)
				{
					DrawModel(model._Model, transform.GetTransform(), viewMatrix, (int)(uint32_t)entity);
				}
			}
		}

		// Render Sprites
		{
			// Renderer::SetCullFace(CullFace::Front);
			auto view = m_Scene->Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& sprite = entity.GetComponent<SpriteRendererComponent>();

				if (sprite.SpriteTexture != nullptr)
				{
					DrawSprite(m_SpriteShader, sprite.SpriteTexture, transform.GetTransform(), sprite.Color, sprite.Reaction, (int)(uint32_t)entity);
				}
			}
			// Renderer::SetCullFace(CullFace::Back);
		}

		// Render Camera Gizmos
		if (renderGizmos)
		{
			Renderer::SetCullFace(CullFace::Front);
			auto view = m_Scene->Registry.view<TransformComponent, CameraComponent>();
			for (auto& e : view)
			{
				Entity entity = Entity(e, m_Scene);

				auto& transform = entity.GetComponent<TransformComponent>();
				auto& camera = entity.GetComponent<CameraComponent>();

				if (camera.Gizmos != nullptr)
				{
					DrawModel(camera.Gizmos, transform.GetTransform() * nlm::scale(nlm::mat4(1.0f), nlm::vec3(1.0f, 1.0f, -1.0f)), viewMatrix, (int)(uint32_t)entity);
				}
			}
			Renderer::SetCullFace(CullFace::Back);
		}

		// Render Skybox?
		//auto clearFlag = camera.GetClearFlagType();
		//if (clearFlag == _Camera::ClearFlagType::Skybox)
		if (camera->GetClearFlagType() == Camera::ClearFlagType::Skybox)
		{
			m_SkyboxShader->Bind();

			Renderer::DepthFunc(DepthComp::Lequal);
			Renderer::SetCullFace(CullFace::Front);

			// tmp
			m_SkyboxTextureCubemap->Bind(0);
			m_SkyboxShader->SetUniformInt("u_Skybox", 0);
			DrawModel(m_Skybox, nlm::translate(nlm::mat4(1.0f), camera->GetPosition()), viewMatrix, -1, m_SkyboxShader);

			Renderer::DepthFunc(DepthComp::Less);
			Renderer::SetCullFace(CullFace::Back);
		}

		// 2D Gizmos
		if (renderGizmos)
		{
			if (!isDeferred)
			{
				Renderer::DepthMask(false);
				Renderer::EnableBlend(true);
			}
			Renderer::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::One);

			auto& pointLightDatas = m_Scene->GetPointLightShadingDataNotConst();
			auto& dirLightDatas = m_Scene->GetDirLightShadingDataNotConst();

			for (auto& l : pointLightDatas)
			{
				DrawSprite(m_GizmosShader, m_PointGizmosTexture, nlm::translate(nlm::mat4(1.0), l.Position) * cameraRotation * nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(l.Color, 0.7), SpriteCameraReaction::LookAt, (int)l.EntityId);
			}
			for (auto& l : dirLightDatas)
			{
				DrawSprite(m_GizmosShader, m_DirGizmosTexture, nlm::translate(nlm::mat4(1.0), l.Position) * cameraRotation * nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(l.Color, 0.7), SpriteCameraReaction::LookAt, (int)l.EntityId);
			}
			Renderer::DepthMask(true);
			Renderer::EnableBlend(false);
			Renderer::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
		}
		

		UpdateParticleSystem();

	}

	void RenderSystem::UpdateParticleSystem()
	{
		bool isDeferred = m_Scene->GetSettingsEntity().GetComponent<SettingsComponent>().RenderMode == RenderMode::Deferred;

		Renderer::DepthMask(false);
		if (!isDeferred)
			Renderer::EnableBlend(true);
		Renderer::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::One);

		auto view = m_Scene->Registry.view<TransformComponent, ParticleSystemComponent>();
		for (auto& e : view)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& particleSystem = entity.GetComponent<ParticleSystemComponent>();
			auto& prop = particleSystem.Prop;

			auto& inputBuffer = particleSystem.TFB[prop.Input];
			auto& outputBuffer = particleSystem.TFB[prop.Output];

			bool isFirstDraw = prop.IsFirstDraw;
			// if is playing
			if ((!prop.IsPaused && m_Scene->IsEditor()) || (m_Scene->IsPlaying() && !m_Scene->IsEditor()) || isFirstDraw)
			{
				// Pass 1
				auto& shader1 = particleSystem.Pass1->GetShader();
				shader1->Bind();
				// shader1->SetUniformMat4("u_View", viewMat);
				// shader1->SetUniformMat4("u_Projection", projMat);
				shader1->SetUniformMat4("u_Transform", transform.GetTransform());
				shader1->SetUniformFloat("u_DeltaTime", m_DeltaTime);
				shader1->SetUniformFloat("u_TotalTime", m_TotalTime);
				shader1->SetUniformFloat("u_Radius", prop.SpawnAreaRadius);
				shader1->SetUniformFloat3("u_MinVelocity", prop.MinVelocity);
				shader1->SetUniformFloat3("u_MaxVelocity", prop.MaxVelocity);
				shader1->SetUniformFloat("u_MinTotalLifetime", prop.MinTotalLifetime);
				shader1->SetUniformFloat("u_MaxTotalLifetime", prop.MaxTotalLifetime);

				Renderer::BindCustomShaderProperties(particleSystem.Pass1);

				inputBuffer->BindBuffer();
				outputBuffer->BindTransformFeedback();

				// Rasterizer discard on
				Renderer::RasterizerDiscard(true);

				Renderer::BeginTransformFeedback_Points();
				// test

				if (prop.IsFirstDraw)
				{
					Renderer::DrawArrays_Points(0, prop.LauncherNum);
					prop.IsFirstDraw = false;
				}
				else
				{
					inputBuffer->Draw_Points();
				}

				Renderer::EndTransformFeedback();

				// Rasterizer discard off
				Renderer::RasterizerDiscard(false);

				shader1->Unbind();
				inputBuffer->UnbindBuffer();
				outputBuffer->UnbindTransformFeedback();
			}

			// Pass 2 Sprite
			auto& shader2 = particleSystem.Pass2->GetShader();
			shader2->Bind();

			shader2->SetUniformMat4("u_Transform", transform.GetTransform());
			shader2->SetUniformInt("u_EntityId", (uint32_t)entity);
			//shader2->SetUniformInt("u_Sprite", 0);
			//particleSystem.Tex->Bind(0);
			Renderer::BindCustomShaderProperties(particleSystem.Pass2);

			outputBuffer->BindBuffer();

			outputBuffer->Draw_Points();

			shader2->Unbind();
			outputBuffer->UnbindBuffer();

			// Switch buffer
			if ((!prop.IsPaused && m_Scene->IsEditor()) || (m_Scene->IsPlaying() && !m_Scene->IsEditor()) || isFirstDraw)
			{
				prop.Input = prop.Output;
				prop.Output = 1 - prop.Output;
			}
		}

		Renderer::DepthMask(true);
		Renderer::EnableBlend(false);
		Renderer::BlendFunc(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
	}

	void RenderSystem::DrawModel(const Ref<Model>& model, const nlm::mat4& transform, const nlm::mat4& viewMatrix, int entityId, const Ref<Shader>& externShader)
	{
		const auto& meshes = model->GetMeshes();
		bool hasBones = model->HasBones();
		std::vector<nlm::mat4> emptyMatrices = {};

		for (const auto& mesh : meshes)
		{
			const auto& material = model->GetMaterial(mesh);

			NL_ENGINE_ASSERT(material, "Material is nullptr!");

			const auto& shader = externShader ? externShader : material->GetShader();
			if (!shader->HasCompiledSuccessfully())
				return;

			NL_ENGINE_ASSERT(shader, "Shader is nullptr!");

			shader->Bind();

			// Typical
			shader->SetUniformMat4("u_Transform", transform);
			//shader->SetUniformMat4("u_NormalMatrix", nlm::transpose(nlm::inverse(transform)));
			// World space normal matrix
			shader->SetUniformMat3("u_NormalMatrixWS", nlm::mat3(nlm::transpose(nlm::inverse(transform))));
			// View space normal matrix
			shader->SetUniformMat3("u_NormalMatrixVS", nlm::mat3(nlm::transpose(nlm::inverse(viewMatrix * transform))));
			shader->SetUniformInt("u_EntityId", entityId);

			// Lights
			// Post-processing...
			if (shader->IsLightingRequired())
			{
				Renderer::BindLightsData(shader, m_Scene->GetPointLightShadingDataNotConst(), m_Scene->GetDirLightShadingDataNotConst());
			}

			if (hasBones)
			{
				shader->SetUniformMat4Array("u_FinalBoneMatrices", model->GetFinalBoneMatrices());
			}

			Renderer::BindCustomShaderProperties(material);

			auto& vertexArray = mesh->GetVertexArray();
			
			vertexArray->Bind();
			Renderer::DrawIndices(vertexArray);

			shader->Unbind();
			vertexArray->Unbind();
		}
	}

	void RenderSystem::DrawSprite(const Ref<Shader>& shader,
		const Ref<Texture2D>& texture,
		const nlm::mat4& transform,
		const nlm::vec4& color,
		SpriteCameraReaction camReact,
		int entityId)
	{
		static Ref<VertexArray> vao = nullptr;
		if (vao == nullptr)
		{
			vao = VertexArray::Create();
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" }
			};
			float vertices[] = { 0.0f, 0.0f, 0.0f };
			Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
			vbo->SetLayout(layout);
			vao->AddVertexBuffer(vbo);
		}

		NL_ENGINE_ASSERT(shader, "Shader is nullptr!");
		NL_ENGINE_ASSERT(texture, "Texture is nullptr!");

		if (!shader->HasCompiledSuccessfully())
			return;

		shader->Bind();

		// Typical
		shader->SetUniformMat4("u_Transform", transform);
		shader->SetUniformInt("u_EntityId", entityId);
		shader->SetUniformFloat4("u_Color", color);
		shader->SetUniformInt("u_Sprite", 0);
		shader->SetUniformInt("u_CameraReaction", (int)camReact);
		texture->Bind(0);

		vao->Bind();

		Renderer::DrawArrays_Points(0, 1);

		shader->Unbind();
		vao->Unbind();
	}

	
}