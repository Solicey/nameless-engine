#pragma once

/*
Ӧ����
*/

#include "Core/App/Window.h"
#include "Core/Misc/Singleton.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Misc/PtrWrapper.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace NL
{
	class Application : public Singleton<Application>
	{
	public:
		Application(token) {}
		~Application();
		Application(const Application&) = delete;
		Application& operator = (const Application&) = delete;

		/// <summary>
		/// �����¼��ص�����
		/// </summary>
		/// <param name="e"></param>
		void OnEvent(Event& e);

		void PushLayer(Ref<Layer> layer);

		void PushOverlay(Ref<Layer> overlay);

		void PopLayer(Ref<Layer> layer);

		Window& GetWindow() { return *m_Window; }

		void Close();

		Ref<ImGuiLayer> GetImGuiLayer() { return m_ImGuiLayer; }

		std::string OpenFileDialogue(const WCHAR* filter)
		{
			return m_Window->OpenFileDialogue(filter);
		}

		std::string SaveFileDialogue(const WCHAR* filter)
		{
			return m_Window->SaveFileDialogue(filter);
		}

		void SetWindowTitle(const std::string& title)
		{
			m_Window->SetWindowTitle(title);
		}

	private:

		/// <summary>
		/// ��ʼ����������AppInitialize����
		/// </summary>
		/// <param name="name">App����</param>
		void Init(const std::string& name);

		/// <summary>
		/// ��main���ã�����ǰҪ��AppInitialize��ʼ��
		/// </summary>
		void Run();

		void Clear();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		friend int ::main(int argc, char** argv);
		friend void AppInitialize(Application& app);

	private:
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		Ref<ImGuiLayer> m_ImGuiLayer;
		float m_LastFrameTime = 0.0f;

		bool m_IsRunning = true;
		bool m_IsMinimize = false;
	};

}