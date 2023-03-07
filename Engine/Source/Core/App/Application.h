#pragma once

/*
Ӧ����
*/

#include "nlpch.h"

#include "Window.h"

#include "Core/Misc/Singleton.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Misc/PtrWrapper.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

int main(int argc, char** argv);

namespace NL
{
	class Application : public Singleton<Application>
	{
	public:
		Application(token) {}
		~Application() = default;
		Application(const Application&) = delete;
		Application& operator = (const Application&) = delete;

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);

		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Close();

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

		void Clean();

		bool OnWindowClose(WindowCloseEvent& e);

		bool OnWindowResize(WindowResizeEvent& e);

	private:
		friend int ::main(int argc, char** argv);
		friend void AppInitialize(Application& app);

	private:
		Scope<Window> m_Window;
		LayerStack m_LayerStack;

		bool m_IsRunning = true;
		bool m_IsMinimize = false;
	};

}