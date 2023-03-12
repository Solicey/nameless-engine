#pragma once

// Tmp
#include "Camera/OrthographicCamera.h"

#include "Core/Misc/PtrWrapper.h"
#include "Resources/Shader.h"
#include "Resources/Model.h"
#include "Renderer/VertexArray.h"
#include "Renderer/RendererAPI.h"

namespace NL
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(
			const Ref<VertexArray>& vertexArray,
			const Ref<Shader>& shader, 
			const nlm::mat4& transform = nlm::mat4(1.0f));

		static void DrawModel(const Ref<Model>& model, 
			const Ref<Shader>& shader,
			const nlm::mat4& transform = nlm::mat4(1.0f));

		struct SceneData
		{
			nlm::mat4 ViewPositionMatrix;
		};

#pragma region Commands

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

#pragma endregion

	private:
		static Scope<SceneData> s_SceneData;
		static Scope<RendererAPI> s_RendererAPI;
	};
}