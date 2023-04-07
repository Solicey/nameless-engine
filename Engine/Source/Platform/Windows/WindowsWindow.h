#pragma once

#include "Core/App/Window.h"
#include "Core/Misc/PtrWrapper.h"

#include "Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace NL
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }
		inline virtual Ref<GraphicsContext> GetGraphicsContext() const override { return m_Context; }

		virtual std::string OpenFileDialogue(const WCHAR* filter) override;
		virtual std::string SaveFileDialogue(const WCHAR* filter) override;

		virtual void SetWindowTitle(const std::string& name) override;

		virtual void HideCursor() override;
		virtual void ShowCursor() override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		Ref<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			EventCallbackFn EventCallback;
			bool VSync;
		} m_Data;

	};
}