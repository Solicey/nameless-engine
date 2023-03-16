#include "nlpch.h"

#include "EditorLayer.h"

#include <Engine.h>
#include <imgui.h>

namespace NL
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
        // Framebuffer Setup
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RedInteger, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

		// m_EditorCamera = EditorCamera(Camera::ProjectionType::Orthographic, 10.0f, 1280, 720, 0.1f, 1000.0f);
		m_EditorCamera = EditorCamera(Camera::ProjectionType::Perspective, 45.0f, 1280, 720, 0.1f, 1000.0f);
		m_EditorScene = CreateRef<Scene>();

		Entity eBox = m_EditorScene->CreateEntity("Box");
		eBox.AddComponent<ModelRendererComponent>("../Assets/Models/Sphere.obj", 
            (uint32_t)eBox,
            ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace);

		Entity eCam = m_EditorScene->CreateEntity("Camera");
		eCam.AddComponent<ModelRendererComponent>("../Assets/Models/Camera.obj", 
            (uint32_t)eCam, 
            ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace);
		eCam.GetComponent<TransformComponent>().SetTranslation(0, 1, 0);

        // Hierarchy
        m_HierarchyPanel = HierarchyPanel(m_EditorScene);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
        // Resize
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            // Camera Controller
            m_EditorCamera.SetAspectRatio(m_ViewportSize.x, m_ViewportSize.y);
        }

        // Framebuffer preparation
        m_Framebuffer->Bind();
        Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Renderer::Clear();
        // Clear entity ID attachment to -1
        m_Framebuffer->ClearAttachment(1, -1);

		//NL_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

        if (m_ViewportHovered)
        {
            m_EditorCamera.OnUpdate(ts);
        }
		
		m_EditorScene->OnUpdateEditor(ts, m_EditorCamera);

        // Check Hovered Entity

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
            m_EntityHovered = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_EditorScene.get());
            m_ViewportHovered = true;
            // NL_ENGINE_TRACE("Viewport Hovered!");
        }
        else
        {
            m_ViewportHovered = false;
        }

        m_Framebuffer->Unbind();

	}

	void EditorLayer::OnImGuiRender()
	{

#pragma region Dockspace

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 110.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

#pragma region MenuBar

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit", "Alt + F4", false))
                    Application::GetInstance().Close();

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Viewport", NULL, &m_ShowViewport);
                ImGui::MenuItem("Hierarchy", NULL, &m_ShowHierarchy);

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

#pragma endregion

#pragma region Windows

        // Hierarchy
        if (m_ShowHierarchy)
        {
            m_HierarchyPanel.OnImGuiRender(m_ShowHierarchy, true);
        }

        // Viewport
        if (m_ShowViewport)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            m_ViewportFocused = ImGui::IsWindowFocused();
            // m_ViewportHovered = ImGui::IsWindowHovered();
            Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportHovered && !m_ViewportFocused);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            // Update Viewport Size
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            // Update Viewport Image
            uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
            ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


            ImGui::End();
            ImGui::PopStyleVar();
        }

#pragma endregion

        ImGui::End();

#pragma endregion

	}

	void EditorLayer::OnEvent(Event& event)
	{
        if (m_ViewportHovered)
        {
            m_EditorCamera.OnEvent(event);
        }

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(NL_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(NL_BIND_EVENT_FN(EditorLayer::OnWindowResizeEvent));
        dispatcher.Dispatch<MouseButtonPressedEvent>(NL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressedEvent));
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		return false;
	}

    bool EditorLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
    {
        return false;
    }

	bool EditorLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
        // Move to Update() Resize
		// m_EditorCamera.SetAspectRatio(event.GetWidth(), event.GetHeight());

		return false;
	}
}