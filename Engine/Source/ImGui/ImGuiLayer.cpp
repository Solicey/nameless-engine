#include "nlpch.h"

#include "ImGuiLayer.h"

#include "Core/Misc/PathConfig.h"
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

    void ImGuiLayer::SetNamelessStyle()
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        std::string fontsFolder = PathConfig::GetInstance().GetFontsFolder().string() + "/Karla-Regular.ttf";
        NL_ENGINE_TRACE("Fonts folder: {0}", fontsFolder);
        ImFont* font = io.Fonts->AddFontFromFileTTF(fontsFolder.c_str(), 20.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    }

    
}