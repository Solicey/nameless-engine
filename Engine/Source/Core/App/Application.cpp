#include "nlpch.h"

#include "Application.h"

#include "Core/Misc/PtrWrapper.h"
#include "Core/Log/Log.h"

namespace NL
{

// �����ڷǾ�̬��Ա������
#define NL_BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(NL_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(NL_BIND_EVENT_FN(Application::OnWindowResize));

		NL_ENGINE_TRACE("{0}", e);
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

	void Application::Init(const std::string& name)
	{
		Log::Init();

		m_Window = Window::Create(WindowProps(name));
		// ���ûص�����
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

	void Application::Clear()
	{
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