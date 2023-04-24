#pragma once

// Tmp
#include "Camera/OrthographicCamera.h"
#include "Camera/EditorCamera.h"

#include "Core/Misc/PtrWrapper.h"
#include "Resources/Shader.h"
#include "Resources/Model.h"
#include "Renderer/VertexArray.h"
#include "Renderer/RendererAPI.h"

namespace NL
{
#define MAX_LIGHT_COUNT 4

	struct PointLightShadingData
	{
		nlm::vec3 Position;
		nlm::vec3 Color;
		bool IsValid = false;
	};

	struct DirLightShadingData
	{
		nlm::vec3 Direction;
		nlm::vec3 Color;
		bool IsValid = false;
	};

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(EditorCamera& camera);
		static void BeginScene(Camera& camera, const nlm::mat4& transform);
		static void EndScene();

		static void Submit(
			const Ref<VertexArray>& vertexArray,
			const Ref<Material>& mat,
			const nlm::mat4& transform = nlm::mat4(1.0f),
			const std::vector<nlm::mat4>& finalMatrices = {}, 
			int entityId = -1,
			bool isSelected = false);

		static void Submit(
			const Ref<VertexArray>& vertexArray,
			const Ref<Shader>& shader, 
			const nlm::mat4& transform = nlm::mat4(1.0f));

		static void SubmitSprite(
			const Ref<VertexArray>& vertexArray,
			const Ref<Shader>& shader,
			const Ref<Texture2D>& texture,
			const nlm::vec4& color,
			const nlm::mat4& transform = nlm::mat4(1.0f),
			int entityId = -1,
			bool isSelected = false);

		static void DrawModel(const Ref<Model>& model, 
			const Ref<Shader>& shader,
			const nlm::mat4& transform);

		static void DrawSprite(const Ref<Shader>& shader,
			const Ref<Texture2D>& texture,
			const nlm::mat4& transform,
			const nlm::vec4& color,
			int entityId = -1,
			bool isSelected = false);

		static void DrawModel(const Ref<Model>& model,
			const nlm::mat4& transform,
			int entityId,
			bool isSelected);

		static void OnWindowResize(unsigned int width, unsigned int height);

		static void SetPointLightData(const PointLightShadingData data[MAX_LIGHT_COUNT])
		{
			for (int i = 0; i < MAX_LIGHT_COUNT; i++)
				s_PointLightDatas[i] = data[i];
		}

		static void SetDirLightData(const DirLightShadingData data[MAX_LIGHT_COUNT])
		{
			for (int i = 0; i < MAX_LIGHT_COUNT; i++)
				s_DirLightDatas[i] = data[i];
		}

		struct SceneData
		{
			nlm::mat4 ViewPositionMatrix;
		};

#pragma region Commands

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetClearColor(const nlm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndices(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndices(vertexArray);
		}

		inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void DepthTest(bool enable)
		{
			s_RendererAPI->DepthTest(enable);
		}

		inline static void DepthFunc(DepthComp comp)
		{
			s_RendererAPI->DepthFunc(comp);
		}

#pragma endregion

	private:
		static Scope<SceneData> s_SceneData;
		static Scope<RendererAPI> s_RendererAPI;

		static PointLightShadingData s_PointLightDatas[MAX_LIGHT_COUNT];
		static DirLightShadingData s_DirLightDatas[MAX_LIGHT_COUNT];

		static Ref<Model> s_Sprite;
	};
}