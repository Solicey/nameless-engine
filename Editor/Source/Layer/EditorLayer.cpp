#include "nlpch.h"

#include "EditorLayer.h"

#include <Engine.h>

namespace NL
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		/*m_VertexArray = VertexArray::Create();

		// Vertex Buffer

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer

		unsigned int indices[3] = { 0, 1, 2 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Square

		m_SquareVA = VertexArray::Create();

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		BufferLayout squareVBLayout = {
			{ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareVBLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = CreateRef<Shader>(vertexSrc, fragmentSrc);*/

		// m_EditorCamera = EditorCamera(Camera::ProjectionType::Orthographic, 10.0f, 1280, 720, 0.1f, 1000.0f);
		m_EditorCamera = EditorCamera(Camera::ProjectionType::Perspective, 45.0f, 1280, 720, 0.1f, 1000.0f);

		std::string normalShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 2) in vec3 a_Normal;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;
			out vec3 v_Normal;
			
			void main()
			{
				v_Position = a_Position;
				v_Normal = a_Normal;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string normalShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec3 v_Normal;
			
			void main()
			{
				color = vec4(v_Normal.x * 0.5 + 0.5, v_Normal.y * 0.5 + 0.5, v_Normal.z * 0.5 + 0.5, 1.0);
			}
		)";

		m_NormalShader = CreateRef<Shader>(normalShaderVertexSrc, normalShaderFragmentSrc);

		// Test Load Model

		m_Box = ModelLoader::Create("../Assets/Models/Box.obj", ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace);
		m_Sphere = ModelLoader::Create("../Assets/Models/Sphere.obj", ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace);
		m_CameraModel = ModelLoader::Create("../Assets/Models/Camera.obj", ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		//NL_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		m_EditorCamera.OnUpdate(ts);

		Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Renderer::Clear();

		// m_Camera.SetPosition(m_CameraPosition);
		// m_Camera.SetRotation(m_CameraRotation);

		Renderer::BeginScene(m_EditorCamera);

		nlm::mat4 transform = nlm::mat4(1.0f);
		transform = nlm::translate(transform, nlm::vec3(1.0f, 1.0f, 0.0f));
		transform = nlm::rotate(transform, nlm::radians(90.0f), nlm::vec3(0.0f, 1.0f, 0.0f));

		// Renderer::Submit(m_SquareVA, m_BlueShader, transform);
		// Renderer::Submit(m_VertexArray, m_Shader);
		// Renderer::DrawModel(m_Box, m_NormalShader);
		Renderer::DrawModel(m_Sphere, m_NormalShader);
		Renderer::DrawModel(m_CameraModel, m_NormalShader, transform);

		Renderer::EndScene();
	}

	void EditorLayer::OnImGuiRender()
	{
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(NL_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(NL_BIND_EVENT_FN(EditorLayer::OnWindowResizeEvent));
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		return false;
	}

	bool EditorLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		m_EditorCamera.SetAspectRatio(event.GetWidth(), event.GetHeight());

		return false;
	}
}