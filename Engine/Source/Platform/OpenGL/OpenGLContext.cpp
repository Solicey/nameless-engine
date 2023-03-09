#include "nlpch.h"

#include "OpenGLContext.h"

#include "Core/Log/Log.h"
#include "Core/Misc/Directives.h"
#include "Core/Misc/PtrWrapper.h"

#include <glad/glad.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>

namespace NL
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		NL_ENGINE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NL_ENGINE_ASSERT(status, "Could not initialize Glad!");

		NL_ENGINE_INFO("OpenGL Info:");
		NL_ENGINE_INFO("  Vendor: {0}", (const char*)(glGetString(GL_VENDOR)));
		NL_ENGINE_INFO("  Renderer: {0}", (const char*)(glGetString(GL_RENDERER)));
		NL_ENGINE_INFO("  Version: {0}", (const char*)(glGetString(GL_VERSION)));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}