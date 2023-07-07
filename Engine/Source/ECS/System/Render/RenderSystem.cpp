#include "nlpch.h"

#include "RenderSystem.h"

#include "Core/Misc/PathConfig.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"
#include "Renderer/Renderer.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Resources/Libraries/UniformBufferLibrary.h"

namespace NL
{
	namespace Utils
	{
		static void CalculateFrustumCornerWorldPos(std::vector<nlm::vec4>& corners, const nlm::mat4& projViewMat)
		{
			const nlm::mat4 inv = nlm::inverse(projViewMat);

			corners.clear();
			for (int x = 0; x < 2; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					for (int z = 0; z < 2; z++)
					{
						nlm::vec4 pt = inv * nlm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
						corners.push_back(pt / pt.w);
					}
				}
			}
		}

		static void CalculateLightSpaceMatrix(nlm::mat4& lightMat, float nearClip, float farClip, float fov, float aspect, const nlm::vec3& lightDir, const nlm::mat4& viewMat, float zMult)
		{
			nlm::mat4 projMat = nlm::perspective(fov, aspect, nearClip, farClip);
			
			std::vector<nlm::vec4> corners;
			CalculateFrustumCornerWorldPos(corners, projMat * viewMat);

			nlm::vec3 center = nlm::vec3(0);
			for (const auto& p : corners)
			{
				center += nlm::vec3(p);
			}
			center /= corners.size();

			auto lightViewMat = nlm::lookAt(center - lightDir, center, nlm::vec3(0.0f, 1.0f, 0.0f));

			float minX = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::min();
			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::min();
			float minZ = std::numeric_limits<float>::max();
			float maxZ = std::numeric_limits<float>::min();

			for (const auto& p : corners)
			{
				const auto tf = lightViewMat * p;
				minX = std::min(minX, tf.x);
				maxX = std::max(maxX, tf.x);
				minY = std::min(minY, tf.y);
				maxY = std::max(maxY, tf.y);
				minZ = std::min(minZ, tf.z);
				maxZ = std::max(maxZ, tf.z);
			}

			// alternative
			if (minZ < 0)
			{
				minZ *= zMult;
			}
			else
			{
				minZ /= zMult;
			}
			if (maxZ < 0)
			{
				maxZ /= zMult;
			}
			else
			{
				maxZ *= zMult;
			}

			nlm::mat4 lightProjMat = nlm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

			lightMat = lightProjMat * lightViewMat;
		}
	}

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
		m_PointGizmosTexture = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Icons/PointLight.png", false);
		m_DirGizmosTexture = Library<Texture2D>::GetInstance().Fetch(assetFolder + "/Icons/DirLight.png", false);

		m_SpriteShader = Library<Shader>::GetInstance().Fetch("Sprite.glsl");

		// Light FBO
		m_CSMShader = Library<Shader>::GetInstance().Fetch("CSM.glsl");
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
			// Renderer::EnableCullFace(false);
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
			// Renderer::EnableCullFace(true);
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

		// Update Light FBO
		{
			auto view = m_Scene->Registry.view<TransformComponent, LightComponent>();
			for (auto& e : view)
			{
				auto [transform, light] = view.get<TransformComponent, LightComponent>(e);
				if (light.Type == LightType::Directional)
				{
					float nearClip = camera->GetPerspectiveNear();
					float farClip = camera->GetPerspectiveFar();
					float fov = camera->GetPerspectiveFOV();
					float aspect = camera->GetAspectRatio();
					const nlm::mat4& viewMat = camera->GetViewMatrix();
					UpdateLightFBO(nearClip, farClip, fov, aspect, viewMat, light, transform);
					break;
				}
			}
		}

		// Render Entities (Opaque)
		{
			// Renderer::EnableCullFace(false);
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
			// Renderer::EnableCullFace(true);
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
				DrawSprite(m_GizmosShader, m_PointGizmosTexture, nlm::translate(nlm::mat4(1.0), l.Position) * cameraRotation * nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(l.Color, 0.5), SpriteCameraReaction::LookAt, (int)l.EntityId);
			}
			for (auto& l : dirLightDatas)
			{
				DrawSprite(m_GizmosShader, m_DirGizmosTexture, nlm::translate(nlm::mat4(1.0), l.Position) * cameraRotation * nlm::scale(nlm::mat4(1.0), nlm::vec3(0.7)), nlm::vec4(l.Color, 0.5), SpriteCameraReaction::LookAt, (int)l.EntityId);
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

			int sampler2DCurrentIndex = 0;

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

			// Shadow Caster
			if (shader->CheckTag(ShaderTag::Shadow))
			{
				shader->SetUniformInt("u_ShadowMaps", sampler2DCurrentIndex);
				Renderer::s_LightFBO->BindTex3D(sampler2DCurrentIndex++);
			}

			if (hasBones)
			{
				shader->SetUniformMat4Array("u_FinalBoneMatrices", model->GetFinalBoneMatrices());
			}

			Renderer::BindCustomShaderProperties(material, sampler2DCurrentIndex);

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

	void RenderSystem::UpdateLightFBO(float nearClip, float farClip, float fov, float aspect, const nlm::mat4& viewMatrix, LightComponent& light, TransformComponent& transform)
	{
		std::vector<float> shadowCascadeLevels{ farClip / 50.0f, farClip / 25.0f, farClip / 10.0f, farClip / 2.0f };
		nlm::vec3 lightDir = transform.GetForward();
		float zMult = m_Scene->GetSettingsEntity().GetComponent<SettingsComponent>().zMult;

		std::vector<nlm::mat4> lightSpaceMatrices;
		int count = shadowCascadeLevels.size();
		for (int i = 0; i < count + 1; i++)
		{
			nlm::mat4 lightMat;
			
			Utils::CalculateLightSpaceMatrix(lightMat, i == 0 ? nearClip : shadowCascadeLevels[i - 1], i == count ? farClip : shadowCascadeLevels[i], fov, aspect, lightDir, viewMatrix, zMult);

			lightSpaceMatrices.push_back(lightMat);
		}

		Ref<UniformBuffer> lightMatricesUBO = Library<UniformBuffer>::GetInstance().GetShadowUniformBuffer();

		static nlm::mat4 emptyMat = nlm::mat4(0);
		static float zero = 0.0;
		int offset = 0;
		for (int i = 0; i < 16; i++)
		{
			lightMatricesUBO->SetData(i < lightSpaceMatrices.size() ? &lightSpaceMatrices[i] : &emptyMat, sizeof(nlm::mat4x4), offset);
			offset += sizeof(nlm::mat4x4);
		}
		for (int i = 0; i < 16; i++)
		{
			lightMatricesUBO->SetData(i < shadowCascadeLevels.size() ? &shadowCascadeLevels[i] : &zero, sizeof(nlm::vec4), offset);
			offset += sizeof(nlm::vec4);
		}
		
		lightMatricesUBO->SetData(&lightDir, sizeof(nlm::vec3), offset);
		offset += sizeof(nlm::vec3);

		lightMatricesUBO->SetData(&light.Intensity, sizeof(float), offset);
		offset += sizeof(float);

		lightMatricesUBO->SetData(&farClip, sizeof(float), offset);
		offset += sizeof(float);

		lightMatricesUBO->SetData(&count, sizeof(int), offset);
		offset += sizeof(int);

		uint32_t currFramebufferID = Renderer::GetDrawFramebuffer();

		Renderer::s_LightFBO->Bind();
		Renderer::SetViewPort(0, 0, Renderer::s_LightFBO->GetSpecification().Width, Renderer::s_LightFBO->GetSpecification().Height);
		Renderer::Clear();
		//Renderer::EnableCullFace(false);
		//Renderer::SetCullFace(CullFace::Front);

		auto view = m_Scene->Registry.view<TransformComponent, ModelRendererComponent>();
		for (auto& e : view)
		{
			Entity entity = Entity(e, m_Scene);

			auto& transform = entity.GetComponent<TransformComponent>();
			auto& model = entity.GetComponent<ModelRendererComponent>();

			if (model._Model != nullptr)
			{
				DrawModel(model._Model, transform.GetTransform(), nlm::mat4(0), -1, m_CSMShader);
			}
		}

		//Renderer::SetCullFace(CullFace::Back);
		//Renderer::EnableCullFace(true);
		Renderer::BindFramebuffer(currFramebufferID);
		const auto& viewportSize = m_Scene->GetSettingsEntity().GetComponent<SettingsComponent>().ViewportSize;
		Renderer::SetViewPort(0, 0, viewportSize.x, viewportSize.y);
		
	}

	
}