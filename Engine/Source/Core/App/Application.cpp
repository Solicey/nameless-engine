#include "nlpch.h"

#include "Application.h"

#include "Core/Misc/PtrWrapper.h"
#include "Core/Log/Log.h"

namespace NL
{

// 适用于非静态成员函数绑定
#define NL_BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	void Application::Init(const std::string& name)
	{
		Log::Init();

		m_Window = Window::Create(WindowProps(name));
		// 设置回调函数
		m_Window->SetEventCallback(NL_BIND_EVENT_FN(Application::OnEvent));

		// ImGui

		// Render Init

	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			// Time

			if (!m_IsMinimize)
			{
				// Update Layers
			}
			
			m_Window->OnUpdate();
		}
	}

	void Application::Clean()
	{
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		return false;
	}

	void Application::OnEvent(Event& e)
	{
	}

	void Application::PushLayer(Layer* layer)
	{
	}

	void Application::PushOverlay(Layer* overlay)
	{
	}

	void Application::PopLayer(Layer* layer)
	{
	}

	void Application::Close()
	{
	}

}