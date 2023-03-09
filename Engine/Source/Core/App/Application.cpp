#include "nlpch.h"

#include "Application.h"

#include "Core/Misc/PtrWrapper.h"
#include "Core/Log/Log.h"

// temp
#include <GLFW/glfw3.h>

namespace NL
{
// 适用于非静态成员函数绑定
#define NL_BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(NL_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(NL_BIND_EVENT_FN(Application::OnWindowResize));

		// NL_ENGINE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Ref<Layer> layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Ref<Layer> overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::PopLayer(Ref<Layer> layer)
	{
		m_LayerStack.PopLayer(layer);
	}

	void Application::Close()
	{

	}

	void Application::Init(const std::string& name)
	{
		Log::Init();

		m_Window = Window::Create(WindowProps(name));
		// 设置回调函数
		m_Window->SetEventCallback(NL_BIND_EVENT_FN(Application::OnEvent));

		// ImGui Init
		m_ImGuiLayer = ImGuiLayer::Create();
		PushOverlay(m_ImGuiLayer);

		// Renderer Init
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			// Time
			float time = (float)glfwGetTime();	// Temp
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_IsMinimize)
			{
				// Update Layers
				for (Ref<Layer> layer : m_LayerStack)
					layer->OnUpdate(timeStep);

				// ImGui Layer
				m_ImGuiLayer->Begin();
				for (Ref<Layer> layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::Clear()
	{
		// Render?
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		return false;
	}

}