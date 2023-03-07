#pragma once

/*
������
*/

#include "nlpch.h"

#include "Core/Misc/PtrWrapper.h"

#include "Events/Event.h"

namespace NL
{
	/// <summary>
	/// ��������
	/// </summary>
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Nameless Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		// �ص�����
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		/// <summary>
		/// ���ûص�����������״�����ã������¼�������Ϊ����
		/// </summary>
		/// <param name="callback"></param>
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		// ��ֱͬ��
		virtual void SetVSync(bool enabled) = 0;

		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}