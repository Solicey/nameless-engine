#pragma once

/*
应用类
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
		/// 处理事件回调函数
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
		/// 初始化函数，由AppInitialize调用
		/// </summary>
		/// <param name="name">App名称</param>
		void Init(const std::string& name);

		/// <summary>
		/// 由main调用，调用前要用AppInitialize初始化
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