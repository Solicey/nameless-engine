#include "nlpch.h"

#include "ImGuiLayer.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLImGuiLayer.h"

#include <imgui.h>

namespace NL
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_BlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            event.Handled |= event.IsInCategory(EventCategory::EventCategoryMouse) & io.WantCaptureMouse;
            event.Handled |= event.IsInCategory(EventCategory::EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    Ref<ImGuiLayer> ImGuiLayer::Create()
    {
        switch (RendererAPI::GetCurrent())
        {
        case RendererAPI::API::None:    return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLImGuiLayer>("OpenGLImGuiLayer");
        }

        NL_ENGINE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    
}