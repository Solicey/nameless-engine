#include "nlpch.h"

#include "EditorLayer.h"

#include <Engine.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <filesystem>
#include <ImGuizmo.h>

namespace NL
{
    namespace Utils
    {
        // from https://github.com/Acmen-Team/Epoch/tree/dev
        template<typename UIFunction>
        static void SceneToolbar(ImGuiDockNode* node, const float DISTANCE, int* corner, UIFunction uiFunc)
        {
            ImGuiIO& io = ImGui::GetIO();
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

            ImVec2 work_area_pos = node->Pos;
            ImVec2 work_area_size = node->Size;

            if (*corner != -1)
            {
                window_flags |= ImGuiWindowFlags_NoMove;
                ImGuiViewport* viewport = ImGui::GetMainViewport();

                ImVec2 window_pos = ImVec2((*corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE), (*corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
                ImVec2 window_pos_pivot = ImVec2((*corner & 1) ? 1.0f : 0.0f, (*corner & 2) ? 1.0f : 0.0f);
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowViewport(node->ID);
            }
            ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background

            uiFunc(corner, work_area_size, window_flags);
        }
    }

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
        // Events
        using EventCallbackFn = std::function<void(Event&)>;
        ScriptGlue::GetInstance().SetEventCallback(NL_BIND_EVENT_FN(EditorLayer::OnEvent));

        // Scene
        m_EditorScene = CreateRef<Scene>();

        // Post-processing
        m_PostProcessing = PostProcessing::Create();

        // m_EditorCamera = EditorCamera(_Camera::ProjectionType::Orthographic, 10.0f, 1280, 720, 0.1f, 1000.0f);
        m_EditorCamera = CreateRef<EditorCamera>(Camera::ProjectionType::Perspective, 45.0f, 1280, 720, 0.1f, 1000.0f);

        // Settings (include framebuffer setup)
        ResetSettingsEntityAfterSceneContextChanged(m_EditorScene);

        // framebuffer Setup
        // UpdateFramebuffer();  

        // G-buffer Setup
        // Albedo+Spec / EntityId / Position / Normal

        // Hierarchy
        m_HierarchyPanel = CreateRef<HierarchyPanel>(m_EditorScene);
        m_HierarchyPanel->SetUpdateRuntimeCameraCallback([this]() { EditorLayer::UpdateRuntimeCameraInfo(); });

        // Icons
        m_PlayButton = Library<Texture2D>::GetInstance().Fetch(PathConfig::GetInstance().GetAssetsFolder().string() + "/Icons/PlayButton.png");
        m_StopButton = Library<Texture2D>::GetInstance().Fetch(PathConfig::GetInstance().GetAssetsFolder().string() + "/Icons/StopButton.png");

        // Scripting
        ScriptEngine::GetInstance().Init();
        m_EditorScene->OnStartEditor();
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
        // Resize
        if (FramebufferSpecification spec = m_MidFramebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_MidFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            // _Camera Controller
            m_EditorCamera->SetAspectRatio(m_ViewportSize.x, m_ViewportSize.y);
        }

        // Update Runtime _Camera. SHOULD BE OPTIMIZED BUT I DONT KNOW HOW TO DO THIS WITH CALLBACKS
        if (!IsEditorMode())
        {
            // m_RuntimeScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            UpdateRuntimeCameraInfo();
        }

        // Framebuffer preparation
        m_MidFramebuffer->Bind();
        /*if (m_EditorCamera.GetClearFlagType() == _Camera::ClearFlagType::Color)
            Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.75f });
        else if (m_EditorCamera.GetClearFlagType() == _Camera::ClearFlagType::Skybox)
            Renderer::SetClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });*/
        Renderer::SetClearColor({ 0, 0, 0, 1 });
        Renderer::Clear();
        m_MidFramebuffer->Unbind();

        // Temp
        if (!IsEditorMode() && m_RuntimeCameraEntity != Entity())
        {
            auto& comp = m_RuntimeCameraEntity.GetComponent<CameraComponent>();
            m_MidFramebuffer->ClearAttachment(0, comp.ClearColor);
        }
        else
            m_MidFramebuffer->ClearAttachment(0, nlm::vec4(0.1f, 0.1f, 0.1f, 0.75));
        m_MidFramebuffer->ClearAttachment(1, -1);
        m_MidFramebuffer->ClearAttachment(2, nlm::vec4(0, 0, 0, 0));

		//NL_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

        m_MidFramebuffer->Bind();
        if (IsEditorMode())
        {
            m_EditorScene->OnUpdateEditor(ts);

            m_EditorCamera->OnUpdate(ts, m_ViewportHovered);

            // TODO: Get editor camera post-processing options.
        }
        else
        {
            if (!m_RuntimeCameraEntity.HasComponent<CameraComponent>() || m_RuntimeCameraEntity == Entity())
            {
                auto camView = m_RuntimeScene->Registry.view<CameraComponent>();
                for (auto entity : camView)
                {
                    m_RuntimeCameraEntity = Entity(entity, m_RuntimeScene.get());
                    break;
                }
            }

            m_RuntimeScene->OnUpdateRuntime(ts, m_IsRuntimeViewportFocused);

            // TODO: Get runtime camera post-processing options.
        }
        m_MidFramebuffer->Unbind();

        // Color Blit 
        m_MidFramebuffer->ColorBlit(0, m_Framebuffer);
        m_MidFramebuffer->ColorBlit(1, m_Framebuffer);
        m_MidFramebuffer->ColorBlit(2, m_Framebuffer);
        m_MidFramebuffer->ColorBlit(3, m_Framebuffer);

        // Check Hovered Entity
        auto [mx, my] = ImGui::GetMousePos();
        // NL_ENGINE_INFO("Hovered pos: ({0}, {1})", mx, my);
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        nlm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;
        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            if (IsEditorMode())
            {
                // Blit
                int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);

                // NL_ENGINE_INFO("pixelData {0}", pixelData);
                m_EntityHovered = (pixelData == -1) ? Entity() : Entity((entt::entity)pixelData, m_EditorScene.get());
            }
            m_ViewportHovered = true;
        }
        else
        {
            m_ViewportHovered = false;
        }        

	}

	void EditorLayer::OnImGuiRender()
	{
        Application::GetInstance().GetImGuiLayer()->BlockEvents(false);

#pragma region Dockspace

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
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

        if (!m_IsRuntimeViewportFocused)
            window_flags |= ImGuiWindowFlags_MenuBar;

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
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

#pragma region MenuBar

        if (!m_IsRuntimeViewportFocused && ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N", false, IsEditorMode()))
                    NewScene();

                if (ImGui::MenuItem("Open", "Ctrl+O", false, IsEditorMode()))
                    OpenScene();

                if (ImGui::MenuItem("Save", "Ctrl+S", false, IsEditorMode()))
                    SaveScene();

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S", false, IsEditorMode()))
                    SaveSceneAs();

                if (ImGui::MenuItem("Exit", "Alt+F4", false))
                    Application::GetInstance().Close();

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Script"))
            {
                if (ImGui::MenuItem("Reload Assembly", "Ctrl+R", false, IsEditorMode()))
                {
                    ScriptEngine::GetInstance().ReloadAssembly();
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Viewport", NULL, &m_ShowViewport);
                ImGui::MenuItem("Hierarchy", NULL, &m_ShowHierarchy);
                ImGui::MenuItem("Scene Settings", NULL, &m_ShowSceneSettings);
                ImGui::MenuItem("Resource List", NULL, &m_ShowResourceList);
                // ImGui::MenuItem("TRS Toolbar", NULL, &m_ShowTRS);

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

#pragma endregion

#pragma region Windows

        // Viewport
        if (m_ShowViewport)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
            
            // Toolbar
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            auto& colors = ImGui::GetStyle().Colors;
            const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
            const auto& buttonActive = colors[ImGuiCol_ButtonActive];
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

            float menuBarHeight, menuBarWidth;
            if (ImGui::BeginMenuBar())
            {
                menuBarWidth = ImGui::GetWindowContentRegionMax().x;
                menuBarHeight = ImGui::GetFrameHeight();

                // Show TRS
                if (IsEditorMode())
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

                    if (ImGui::RadioButton("T", m_GuizmoType == ImGuizmo::OPERATION::TRANSLATE))
                    {
                        m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
                    }
                    ImGui::SameLine();
                    if (ImGui::RadioButton("R", m_GuizmoType == ImGuizmo::OPERATION::ROTATE))
                    {
                        m_GuizmoType = ImGuizmo::OPERATION::ROTATE;
                    }
                    ImGui::SameLine();
                    if (ImGui::RadioButton("S", m_GuizmoType == ImGuizmo::OPERATION::SCALE))
                    {
                        m_GuizmoType = ImGuizmo::OPERATION::SCALE;
                    }
                    ImGui::SameLine();

                    ImGui::PopStyleVar();
                }
                // Runtime but not focused
                // Show Runtime _Camera
                else if (!m_IsRuntimeViewportFocused)
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                    ImGui::PushItemWidth(menuBarWidth * 0.2f);

                    auto& runtimeCameraName = m_RuntimeCameraEntity.GetName();
                    if (ImGui::BeginCombo("Runtime Camera", runtimeCameraName.c_str()))
                    {
                        Application::GetInstance().GetImGuiLayer()->BlockEvents(true);
                        auto camView = m_RuntimeScene->Registry.view<CameraComponent>();
                        for (auto entity : camView)
                        {
                            auto& comp = m_RuntimeScene->Registry.get<CameraComponent>(entity);
                            auto& name = m_RuntimeScene->Registry.get<IdentityComponent>(entity).Name;
                            bool isSelected = entity == m_RuntimeCameraEntity;
                            if (ImGui::Selectable(name.c_str(), isSelected))
                            {
                                if (!isSelected)
                                {
                                    m_RuntimeCameraEntity = Entity(entity, m_RuntimeScene.get());
                                    // UpdateRuntimeCameraInfo();
                                }
                            }
                        }

                        ImGui::EndCombo();
                    }

                    ImGui::PopStyleVar();
                    ImGui::PopItemWidth();
                }
                else
                {
                    ImGui::Text("Press 'Esc' to escape!");
                    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - (menuBarHeight * 3.0f));
                    ImGui::Text("FPS: %.1f", io.Framerate);
                }

                // Play & Stop Button
                if (!m_IsRuntimeViewportFocused)
                {
                    Ref<Texture2D> icon = IsEditorMode() ? m_PlayButton : m_StopButton;
                    ImGui::SetCursorPosX((menuBarWidth * 0.5f) - (menuBarHeight * 0.5f));
                    if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(menuBarHeight, menuBarHeight), ImVec2(0, 0), ImVec2(1, 1), 0))
                    {
                        if (IsEditorMode())
                            OnScenePlay();
                        else
                            OnSceneStop();
                    }

                    ImGui::SameLine();

                    // Maximize On Play
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - (menuBarHeight * 4.5f));
                    if (ImGui::Checkbox("Maximize", &m_IsMaximizeOnPlay))
                    {
                    }
                    ImGui::PopStyleVar();
                }

                ImGui::EndMenuBar();
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor(3);

            // FPS
            // ImGui::Text("FPS: %.1f", io.Framerate);

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            // Temp
            bool showGizmos = m_Settings.GetComponent<SettingsComponent>().ShowGizmos;
            // Grid
            if (IsEditorMode() && showGizmos && m_EditorCamera->GetClearFlagType() == Camera::ClearFlagType::Color)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
                const nlm::mat4& cameraProjection = m_EditorCamera->GetProjectionMatrix();
                const nlm::mat4 cameraView = m_EditorCamera->GetViewMatrix();
                ImGuizmo::DrawGrid(nlm::value_ptr(cameraView), nlm::value_ptr(cameraProjection), nlm::value_ptr(nlm::mat4(1.0f)), 20.0f);
            }
            
            m_ViewportFocused = ImGui::IsWindowFocused();
            // m_ViewportHovered = ImGui::IsWindowHovered();
            // Application::GetInstance().GetImGuiLayer()->BlockEvents(false);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            // Update Viewport Size
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            // Update Viewport Image
            uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();

            // uint64_t textureID = Library<Texture2D>::GetInstance().Get("../Assets/Models/nanosuit/arm_dif.png")->GetRendererID();
            if (!IsEditorMode())
            {
                float xRatio = (m_ViewportSize.x - m_RuntimeAspect.x) / 2.0f;
                float yRatio = (m_ViewportSize.y - m_RuntimeAspect.y) / 2.0f;
                ImGui::SetCursorPos(ImVec2{ ImGui::GetWindowContentRegionMin().x + xRatio, ImGui::GetWindowContentRegionMin().y + yRatio });
                // float dx = (xRatio < 0) ? (-xRatio) / m_RuntimeAspect.x : 0;
                // float dy = (yRatio < 0) ? (-yRatio) / m_RuntimeAspect.y : 0;
                // ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_RuntimeAspect.x, m_RuntimeAspect.y }, ImVec2{ 0 + dx, 1 - dy }, ImVec2{ 1 - dx, 0 + dy });
                // 
                // Bugs remain... Smaller viewport leading to lower graphic performances...
                if (m_RuntimeCameraEntity.HasComponent<PostProcessingComponent>())
                {
                    auto& comp = m_RuntimeCameraEntity.GetComponent<PostProcessingComponent>();
                    textureID = m_PostProcessing->ExecutePostProcessingQueue(comp.Queue, m_Framebuffer, m_HierarchyPanel->GetSelectedEntityId(), m_RuntimeScene->GetPointLightShadingDataNotConst(), m_RuntimeScene->GetDirLightShadingDataNotConst());
                }
                ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_RuntimeAspect.x, m_RuntimeAspect.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            }
            else
            {
                if (m_Settings.HasComponent<PostProcessingComponent>())
                    textureID = m_PostProcessing->ExecutePostProcessingQueue(m_Settings.GetComponent<PostProcessingComponent>().Queue, m_Framebuffer, m_HierarchyPanel->GetSelectedEntityId(), m_EditorScene->GetPointLightShadingDataNotConst(), m_EditorScene->GetDirLightShadingDataNotConst());
                ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
            }

            // Gizmos
            if (IsEditorMode() && showGizmos)
            {
                Entity entitySelected = m_HierarchyPanel->GetSelectedEntity();
                if (entitySelected && entitySelected.HasComponent<TransformComponent>())
                {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();
                    ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                    // Editor camera
                    const nlm::mat4& cameraProjection = m_EditorCamera->GetProjectionMatrix();
                    const nlm::mat4 cameraView = m_EditorCamera->GetViewMatrix();

                    auto& component = entitySelected.GetComponent<TransformComponent>();
                    nlm::mat4 transform = component.GetTransform();

                    // Snap
                    bool snap = Input::IsKeyPressed(Key::LeftControl);
                    float snapValue = 0.5f; // Snap to 0.5m for translation/scale
                    // Snap to 45 degrees for rotation
                    if (m_GuizmoType == ImGuizmo::OPERATION::ROTATE)
                        snapValue = 45.0f;

                    float snapValues[3] = { snapValue, snapValue, snapValue };

                    ImGuizmo::AllowAxisFlip(false);
                    ImGuizmo::Manipulate(nlm::value_ptr(cameraView), nlm::value_ptr(cameraProjection),
                        (ImGuizmo::OPERATION)m_GuizmoType, ImGuizmo::LOCAL, nlm::value_ptr(transform),
                        nullptr, snap ? snapValues : nullptr);

                    m_TRSEntity = entitySelected;

                    if (ImGuizmo::IsUsing())
                    {
                        nlm::vec3 translation, rotation, scale, skew;
                        nlm::vec4 persp;
                        nlm::quat orien;
                        nlm::decompose(transform, scale, orien, translation, skew, persp);

                        rotation = nlm::eulerAngles(orien);
                        nlm::vec3 deltaRotation = rotation - component.Rotation;

                        component.Translation = translation;
                        component.Rotation += deltaRotation;
                        component.Scale = scale;
                    }
                }
            }

            // Mouse Cursor Capture
            /*if (!IsEditorMode() && m_IsRuntimeViewportFocused)
            {
                Application::GetInstance().HideCursor();                
            }
            else
            {
                Application::GetInstance().ShowCursor();
            }*/

            ImGui::End();
            ImGui::PopStyleVar();
        }

        // Hierarchy
        if (m_ShowHierarchy && !(m_IsMaximizeOnPlay && m_ViewportMode == ViewportMode::Runtime))
        {
            m_HierarchyPanel->OnImGuiRender(m_ShowHierarchy, true);
        }

        // Resource List
        if (m_ShowResourceList && !(m_IsMaximizeOnPlay && m_ViewportMode == ViewportMode::Runtime))
        {
            ImGui::Begin("Resource List");

            std::string texString = "Texture (" + std::to_string(Library<Texture2D>::GetInstance().GetSize()) + ")";
            std::string meshString = "Mesh (" + std::to_string(Library<Mesh>::GetInstance().GetSize()) + ")";
            std::string shaderString = "Shader (" + std::to_string(Library<Shader>::GetInstance().GetSize()) + ")";

            if (ImGui::TreeNodeEx("##TexturesList", 0, texString.c_str()))
            {
                const auto& names = Library<Texture2D>::GetInstance().GetNames();
                for (auto& name : names)
                    ImGui::Text(name.c_str());
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("##MeshesList", 0, meshString.c_str()))
            {
                const auto& names = Library<Mesh>::GetInstance().GetNames();
                for (auto& name : names)
                    ImGui::Text(name.c_str());
                ImGui::TreePop();
            }

            if (ImGui::TreeNodeEx("##ShadersList", 0, shaderString.c_str()))
            {
                const auto& names = Library<Shader>::GetInstance().GetNames();
                for (auto& name : names)
                    ImGui::Text(name.c_str());
                ImGui::TreePop();
            }

            ImGui::End();
        }

        // Scene Settings
        if (m_ShowSceneSettings && !(m_IsMaximizeOnPlay && m_ViewportMode == ViewportMode::Runtime))
        {
            ImGui::Begin("Scene Settings");

            auto& settings = m_Settings.GetComponent<SettingsComponent>();

            bool hasRenderModeModified = false;
            if (ImGui::TreeNode("Render Mode"))
            {
                if (ImGui::RadioButton("Forward", (settings.RenderMode == RenderMode::Forward)))
                {
                    settings.RenderMode = RenderMode::Forward;
                    hasRenderModeModified = true;
                }
                ImGui::SameLine();
                if (ImGui::RadioButton("Deferred", (settings.RenderMode == RenderMode::Deferred)))
                {
                    settings.RenderMode = RenderMode::Deferred;
                    settings.AntiAliasingType = AntiAliasingType::None;
                    hasRenderModeModified = true;
                }
                ImGui::TreePop();
            }
            if (hasRenderModeModified)
            {
                UpdateFramebuffer();
            }

            // Post-processing
            if (ImGui::TreeNode("Editor Camera Post-processing"))
            {
                PostProcessingComponent& comp = m_Settings.GetComponent<PostProcessingComponent>();
                auto& queue = comp.Queue;

                if (ImGui::BeginTabBar("Shader Pass", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyResizeDown))
                {
                    if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                    {
                        // Check if shader is nullptr in Post Processing
                        queue.push_back(CreateRef<Material>());
                    }

                    for (int i = 0; i < queue.size();)
                    {
                        auto& mat = queue[i];
                        bool open = true;
                        std::string name = std::to_string(i) + "." + mat->GetShaderNameNoSuffix();
                        if (ImGui::BeginTabItem(name.c_str(), &open, ImGuiTabItemFlags_None))
                        {
                            //ImGui::Text("This is the %d tab!", i);
                            /*HierarchyPanel::DrawShaderCombo<PostProcessingComponent>(mat, mat->GetName(), m_HierarchyPanel->m_ShaderSelectOpen, [](auto& comp, const std::string& shaderName)
                                {
                                    PostProcessingComponent& postcomp = comp;
                                    postcomp.UpdateShaderProperties(shaderName);
                                }, ShaderUse::PostProcess, m_EditorScene, false);*/
                            m_HierarchyPanel->DrawEditorCameraPostProcessShaderCombo(mat);
                            ImGui::EndTabItem();
                        }

                        if (!open)
                        {
                            mat->DeleteTexturesAndShadersReference();
                            queue.erase(queue.begin() + i);
                        }
                        else
                            i++;
                    }

                    ImGui::EndTabBar();
                }

                ImGui::TreePop();
            }

            // Anti-Aliasing
            bool hasAntiAliasModified = false;
            if (ImGui::TreeNode("Anti-aliasing"))
            {
                if (ImGui::RadioButton("None", (settings.AntiAliasingType == AntiAliasingType::None)))
                {
                    settings.AntiAliasingType = AntiAliasingType::None;
                    hasAntiAliasModified = true;
                }
                if (settings.RenderMode != RenderMode::Deferred)
                {
                    ImGui::SameLine();
                    if (ImGui::RadioButton("2x MSAA", (settings.AntiAliasingType == AntiAliasingType::MSAA && settings.MSAASamples == 2)))
                    {
                        settings.AntiAliasingType = AntiAliasingType::MSAA;
                        settings.MSAASamples = 2;
                        hasAntiAliasModified = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::RadioButton("4x MSAA", (settings.AntiAliasingType == AntiAliasingType::MSAA && settings.MSAASamples == 4)))
                    {
                        settings.AntiAliasingType = AntiAliasingType::MSAA;
                        settings.MSAASamples = 4;
                        hasAntiAliasModified = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::RadioButton("8x MSAA", (settings.AntiAliasingType == AntiAliasingType::MSAA && settings.MSAASamples == 8)))
                    {
                        settings.AntiAliasingType = AntiAliasingType::MSAA;
                        settings.MSAASamples = 8;
                        hasAntiAliasModified = true;
                    }
                }
                ImGui::TreePop();
            }
            if (hasAntiAliasModified)
            {
                UpdateFramebuffer();
            }

            // Editor _Camera Clear Flag
            if (ImGui::TreeNode("Clear Flag"))
            {
                Camera::ClearFlagType type = settings.EditorCameraClearFlag;
                if (ImGui::RadioButton("Color", (type == Camera::ClearFlagType::Color)))
                {
                    settings.EditorCameraClearFlag = Camera::ClearFlagType::Color;
                    m_EditorCamera->SetClearFlagType(settings.EditorCameraClearFlag);
                }
                ImGui::SameLine();
                if (ImGui::RadioButton("Skybox", (type == Camera::ClearFlagType::Skybox)))
                {
                    settings.EditorCameraClearFlag = Camera::ClearFlagType::Skybox;
                    m_EditorCamera->SetClearFlagType(settings.EditorCameraClearFlag);
                }
                ImGui::TreePop();
            }

            // Editor Viewport Gizmos
            if (ImGui::TreeNode("Gizmos"))
            {
                if (ImGui::Checkbox("Show Gizmos", &settings.ShowGizmos))
                {
                    /*if (m_EditorCamera.IsRenderGizmos())
                        m_EditorPostProcessingQueue = { CreateRef<Material>("EditorOutline.glsl") };
                    else m_EditorPostProcessingQueue.clear();*/
                }
                ImGui::TreePop();
            }

            // Runtime PostEffects

            ImGui::End();
        }

#pragma endregion

        ImGui::End();

#pragma endregion

	}

	void EditorLayer::OnEvent(Event& event)
	{
        if (m_ViewportHovered)
        {
            // NL_ENGINE_INFO("Editor _Camera Event");
            m_EditorCamera->OnEvent(event);
        }

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(NL_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(NL_BIND_EVENT_FN(EditorLayer::OnWindowResizeEvent));
        dispatcher.Dispatch<MouseButtonPressedEvent>(NL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(NL_BIND_EVENT_FN(EditorLayer::OnMouseMovedEvent));

        dispatcher.Dispatch<RuntimeCameraSwitchedEvent>(NL_BIND_EVENT_FN(EditorLayer::OnRuntimeCameraSwitched));

	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
        if (event.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);
        bool mouseRight = Input::IsMouseButtonPressed(Mouse::ButtonRight);
        switch (event.GetKeyCode())
        {
        // Files
        case Key::N:
        {
            if (control && IsEditorMode())
                NewScene();
            break;
        }
        case Key::O:
        {
            if (control && IsEditorMode())
                OpenScene();
            break;
        }
        case Key::S:
        {
            if (control && IsEditorMode())
            {
                if (shift)
                    SaveSceneAs();
                else
                    SaveScene();
            }
            else if (shift)
            {
                if (IsEditorMode())
                    m_GuizmoType = ImGuizmo::OPERATION::SCALE;
            }
            break;
        }
        case Key::R:
        {
            if (control && IsEditorMode())
            {
                if (IsEditorMode())
                {
                    ScriptEngine::GetInstance().ReloadAssembly();
                }
            }
            else if (shift)
            {
                if (IsEditorMode())
                    m_GuizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            break;
        }
        case Key::T:
        {
            if (shift)
            {
                if (IsEditorMode())
                    m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
        }
        // Focus
        case Key::F:
        {
            if (IsEditorMode() && (m_ViewportFocused || m_ViewportHovered))
            {
                Entity selectedEntity = m_HierarchyPanel->GetSelectedEntity();
                if (selectedEntity)
                {
                    // Ought to have transform...
                    m_EditorCamera->SetCenter(selectedEntity.GetComponent<TransformComponent>().Translation);
                }
            }
            break;
        }
        case Key::Escape:
        {
            if (!IsEditorMode() && m_IsRuntimeViewportFocused)
            {
                m_IsRuntimeViewportFocused = false;
                Application::GetInstance().ShowCursor();
                // OnSceneStop();
            }
            break;
        }
        case Key::D:
        {
            if (shift)
            {
                Entity entity = m_HierarchyPanel->GetSelectedEntity();
                if (entity != Entity())
                {
                    IsEditorMode() ? m_EditorScene->DestroyEntity(entity) : m_RuntimeScene->DestroyEntity(entity);
                    m_HierarchyPanel->SetSelectedEntity(Entity());
                }
            }
            else if (control)
            {
                Entity entity = m_HierarchyPanel->GetSelectedEntity();
                if (entity != Entity())
                {
                    m_HierarchyPanel->SetSelectedEntity(IsEditorMode() ? m_EditorScene->DuplicateEntity(entity) : m_RuntimeScene->DuplicateEntity(entity));
                }
            }
            break;
        }
        case Key::A:
        {
            if (shift)
            {
                m_HierarchyPanel->SetSelectedEntity(IsEditorMode() ? m_EditorScene->CreateEntity() : m_RuntimeScene->CreateEntity());
            }
            break;
        }
        }

		return false;
	}

    bool EditorLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
    {
        // if (!m_ViewportFocused && !m_ViewportHovered)
            // return false;
        if (event.GetMouseButton() == Mouse::ButtonLeft)
        {
            // !ImGuizmo::IsOver()
            if (IsEditorMode() && m_ViewportHovered && !(ImGuizmo::IsOver() && m_TRSEntity == m_HierarchyPanel->GetSelectedEntity()))
            {
                m_HierarchyPanel->SetSelectedEntity(m_EntityHovered);
                // Click a viewport entity
                if (m_EntityHovered)
                {
                    NL_TRACE("Viewport select entity: {0}, {1}", m_EntityHovered.GetName(), (uint32_t)m_EntityHovered);
                }
            }
            else if (!IsEditorMode() && m_ViewportHovered && !m_IsRuntimeViewportFocused)
            {
                auto [mx, my] = ImGui::GetMousePos();
                if (mx >= m_ViewportBounds[0].x && mx <= m_ViewportBounds[1].x &&
                    my >= m_ViewportBounds[0].y && my <= m_ViewportBounds[1].y)
                {
                    m_IsRuntimeViewportFocused = true;
                    Application::GetInstance().HideCursor();
                }
            }
        }
        return false;
    }

	bool EditorLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
        // Move to Update() Resize
		// m_EditorCamera.SetAspectRatio(event.GetWidth(), event.GetHeight());

		return false;
	}

    bool EditorLayer::OnMouseMovedEvent(MouseMovedEvent& event)
    {
        // NL_INFO("Mouse Pos: ({0}, {1})", event.GetX(), event.GetY());
        return false;
    }

    bool EditorLayer::OnRuntimeCameraSwitched(RuntimeCameraSwitchedEvent& event)
    {
        // NL_INFO("Runtime _Camera Switch!");

        m_RuntimeCameraEntity = event.GetEntity();

        return false;
    }

    void EditorLayer::SerializeScene(Ref<Scene> scene, const std::string& path)
    {
        SceneSerializer serializer(scene);
        /*std::unordered_map<std::string, int> sceneSettingsInt = {
            {"AntiAliasingType", (int)m_AntiAliasingType},
            {"MSAASamples", m_MSAASamples},
            {"EditorCameraClearFlag", (int)m_EditorCamera.GetClearFlagType()},
            {"ShowGizmos", m_EditorCamera.IsRenderGizmos() ? 1 : 0}
        };*/

        // Temp
        auto& transform = m_Settings.GetComponent<TransformComponent>();
        transform.Translation = m_EditorCamera->GetPosition();
        transform.Rotation = nlm::eulerAngles(m_EditorCamera->GetOrientation());
        transform.Scale = m_EditorCamera->GetFocalPoint();

        serializer.Serialize(path);
    }

    void EditorLayer::NewScene()
    {
        if (m_ViewportMode == ViewportMode::Runtime)
        {
            OnSceneStop();
        }

        // if (m_EditorScene)
           // m_EditorScene->Registry.clear();
        m_EditorScene->DestroyScene();
        m_EditorScene.reset();
        m_EditorScene = CreateRef<Scene>();

        m_EditorScene->OnStartEditor();

        // Switch Scene Context
        m_HierarchyPanel->SetSceneContext(m_EditorScene);
        ResetSettingsEntityAfterSceneContextChanged(m_EditorScene);

        m_EditorScenePath = "";

        // Avoid Memory Leak
        // Library<Texture2D>::GetInstance().TraverseDelete();
        Application::GetInstance().SetWindowTitle("Nameless Editor - New Scene");
    }

    void EditorLayer::OpenScene()
    {
        if (m_ViewportMode == ViewportMode::Runtime)
        {
            OnSceneStop();
        }

        std::string path = Application::GetInstance().OpenFileDialogue(L"NL Scene(*.nl)\0*.nl\0\0");
        if (!path.empty())
            OpenScene(path);

        // Avoid Memory Leak
        // Library<Texture2D>::GetInstance().TraverseDelete();
    }

    void EditorLayer::OpenScene(const std::string& path)
    {
        if (path.find(".nl") == path.npos)
		{
			NL_WARN("Could not load {0}, it's NOT a scene file", path);
			return;
		}

        Ref<Scene> newScene = CreateRef<Scene>();
        SceneSerializer serializer(newScene);
        /*std::unordered_map<std::string, int> sceneSettingsInt = {
            {"AntiAliasingType", (int)m_AntiAliasingType},
            {"MSAASamples", m_MSAASamples},
            {"EditorCameraClearFlag", (int)m_EditorCamera.GetClearFlagType()},
            {"ShowGizmos", m_EditorCamera.IsRenderGizmos() ? 1 : 0}
        };*/
        if (serializer.Deserialize(path))
        {
            // if (m_EditorScene)
               // m_EditorScene->Registry.clear();

            m_EditorScene->DestroyScene();
            m_EditorScene.reset();
            m_EditorScene = newScene;            
            m_EditorScenePath = path;

            m_EditorScene->OnStartEditor();

            // Switch Scene Context
            m_HierarchyPanel->SetSceneContext(m_EditorScene);
            ResetSettingsEntityAfterSceneContextChanged(m_EditorScene);

            Application::GetInstance().SetWindowTitle("Nameless Editor - " + path.substr(path.find_last_of("/\\") + 1));
            
        }
    }

    void EditorLayer::SaveScene()
    {
        if (m_ViewportMode == ViewportMode::Runtime)
        {
            OnSceneStop();
        }

        if (!m_EditorScenePath.empty())
            SerializeScene(m_EditorScene, m_EditorScenePath);
        else
            SaveSceneAs();
        NL_TRACE("Scene saved!");
    }

    void EditorLayer::SaveSceneAs()
    {
        if (m_ViewportMode == ViewportMode::Runtime)
        {
            OnSceneStop();
        }

        std::string path = Application::GetInstance().SaveFileDialogue(L"NL Scene(*.nl)\0*.nl\0\0");
        if (!path.empty())
        {
            // force surffix
            if (path.find(".nl") == path.npos)
            {
                std::filesystem::path filepath(path);
                path = path + ".nl";
                filepath.replace_filename(path);
            }
            // Switch Scene Context
            SerializeScene(m_EditorScene, path);
            m_EditorScenePath = path;

            Application::GetInstance().SetWindowTitle("Nameless Editor - " + path.substr(path.find_last_of("/\\") + 1));
        }
    }

    void EditorLayer::OnScenePlay()
    {
        m_ViewportMode = ViewportMode::Runtime;

        m_RuntimeScene = Scene::DuplicateScene(m_EditorScene);
        m_RuntimeScene->OnStartRuntime();

        m_HierarchyPanel->SetSceneContext(m_RuntimeScene);
        ResetSettingsEntityAfterSceneContextChanged(m_RuntimeScene);
    }
    
    void EditorLayer::OnSceneStop()
    {
        m_ViewportMode = ViewportMode::Editor;

        m_RuntimeScene->OnStopRuntime(m_EditorScene.get());
        m_RuntimeScene->DestroyScene();
        m_RuntimeScene.reset();

        m_RuntimeCameraEntity = {};

        m_HierarchyPanel->SetSceneContext(m_EditorScene);
        ResetSettingsEntityAfterSceneContextChanged(m_EditorScene);
    }

    void EditorLayer::UpdateRuntimeCameraInfo()
    {
        if (m_RuntimeCameraEntity.HasComponent<CameraComponent>())
        {
            auto& cam = m_RuntimeCameraEntity.GetComponent<CameraComponent>();
            if (cam.FixedAspectRatio)
            {
                this->m_RuntimeAspect = nlm::vec2(cam._Camera->GetViewportWidth(), cam._Camera->GetViewportHeight());
            }
            else
            {
                this->m_RuntimeAspect = nlm::vec2((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
                cam._Camera->SetAspectRatio((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
            }

            m_Settings.GetComponent<SettingsComponent>().RuntimeCameraID = m_RuntimeCameraEntity.GetID();
        }
    }

    void EditorLayer::UpdateFramebuffer()
    {
        auto& settings = m_Settings.GetComponent<SettingsComponent>();
        switch (settings.RenderMode)
        {
        case RenderMode::Forward:
        {
            FramebufferSpecification msSpec;
            msSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RedInteger, FramebufferTextureFormat::Depth };
            msSpec.Width = 1280;
            msSpec.Height = 720;
            if (settings.AntiAliasingType == AntiAliasingType::None)
            {
                msSpec.Samples = 1;
            }
            else if (settings.AntiAliasingType == AntiAliasingType::MSAA)
            {
                msSpec.Samples = settings.MSAASamples;
            }
            m_MidFramebuffer = Framebuffer::Create(msSpec);

            msSpec.Samples = 1;
            m_Framebuffer = Framebuffer::Create(msSpec);

            break;
        }
        case RenderMode::Deferred:
        {
            FramebufferSpecification msSpec;
            msSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RedInteger, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth };
            msSpec.Width = 1280;
            msSpec.Height = 720;
            msSpec.Samples = 1;
            m_MidFramebuffer = Framebuffer::Create(msSpec);
            m_Framebuffer = Framebuffer::Create(msSpec);
            break;
        }
        default:
            break;
        }

    }

    void EditorLayer::ResetSettingsEntityAfterSceneContextChanged(Ref<Scene>& scene)
    {
        m_Settings = scene->GetSettingsEntity();
        if (m_Settings == Entity())
        {
            m_Settings = scene->CreateEntity(m_SettingsEntityName);
            m_Settings.AddComponent<SettingsComponent>();
            m_Settings.AddComponent<PostProcessingComponent>().Queue.push_back(CreateRef<Material>("EditorOutline.glsl"));

            // Set editor camera transform
            auto& transform = m_Settings.GetComponent<TransformComponent>();
            auto pos = transform.GetTranslation();
            NL_ENGINE_INFO("set editor cam pos: ({0}, {1}, {2})", pos.x, pos.y, pos.z);

            transform.Translation = m_EditorCamera->GetPosition();
            transform.Rotation = nlm::eulerAngles(m_EditorCamera->GetOrientation());
            transform.Scale = m_EditorCamera->GetFocalPoint();
        }
        else
        {
            auto& transform = m_Settings.GetComponent<TransformComponent>();
            m_EditorCamera->SetState(transform.GetTranslation(), transform.GetRotation(), transform.GetScale());
            auto pos = transform.GetTranslation();
            NL_ENGINE_INFO("set editor cam pos: ({0}, {1}, {2})", pos.x, pos.y, pos.z);
        }

        auto& setComp = m_Settings.GetComponent<SettingsComponent>();

        setComp.EditorCamera = m_EditorCamera;
        setComp.RuntimeCameraID = m_RuntimeCameraEntity.GetID();
        NL_ENGINE_INFO("Runtime Camera ID: {0}", m_RuntimeCameraEntity.GetID());
        // Temp
        m_EditorCamera->SetClearFlagType(setComp.EditorCameraClearFlag);
        UpdateFramebuffer();
    }

}