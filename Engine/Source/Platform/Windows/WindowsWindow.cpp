#include "nlpch.h"

#include "WindowsWindow.h"

#include "Core/Log/Log.h"

namespace NL
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		NL_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

#ifdef NL_PLATFORM_WINDOWS
	/// <summary>
	/// 实现Window类中的Create函数
	/// TODO：跨平台
	/// </summary>
	/// <param name="props"></param>
	/// <returns></returns>
	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}
#endif

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		NL_ENGINE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);
	}

	void WindowsWindow::Shutdown()
	{
	}
}