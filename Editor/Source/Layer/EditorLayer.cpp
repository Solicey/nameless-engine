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
        // Framebuffer Setup
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RedInteger, FramebufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);

		// m_EditorCamera = EditorCamera(Camera::ProjectionType::Orthographic, 10.0f, 1280, 720, 0.1f, 1000.0f);
		m_EditorCamera = EditorCamera(Camera::ProjectionType::Perspective, 45.0f, 1280, 720, 0.1f, 1000.0f);
		m_EditorScene = CreateRef<Scene>();

        /*Entity eCam = m_EditorScene->CreateEntity("Camera");
        eCam.AddComponent<ModelRendererComponent>("../Assets/Models/Camera.obj",
            (int)(uint32_t)(eCam),
            ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace);
        eCam.GetComponent<TransformComponent>().SetTranslation(0, 1, 0);

		Entity eBox = m_EditorScene->CreateEntity("Box");
		eBox.AddComponent<ModelRendererComponent>("../Assets/Models/Box.obj", 
            (int)(uint32_t)(eBox),
            ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace);

        Entity eSphere = m_EditorScene->CreateEntity("Sphere");
        eSphere.AddComponent<ModelRendererComponent>("../Assets/Models/Sphere.obj",
            (int)(uint32_t)(eSphere),
            ModelLoaderFlags::Triangulate | ModelLoaderFlags::FlipUVs | ModelLoaderFlags::CalcTangentSpace);
        eSphere.GetComponent<TransformComponent>().SetTranslation(0, -1, 0);*/

        // Hierarchy
        m_HierarchyPanel = CreateRef<HierarchyPanel>(m_EditorScene);
        m_HierarchyPanel->SetUpdateRuntimeCameraCallback([this]() { EditorLayer::UpdateRuntimeAspect(); });


        // Test mono
        /*auto& scripting = ScriptEngine::GetInstance();
        MonoAssembly* assembly = scripting.LoadCSharpAssembly("../ScriptCore/Scripts/ScriptCore.dll");
        MonoClass* testClass = scripting.GetClassInAssembly(assembly, "TestNamespace", "TestClass");
        MonoObject* testClassInstance = scripting.Instantiate(testClass);
        scripting.CallMethod(testClassInstance, "PrintFloatVar");*/

        // Icons
        m_PlayButton = Texture2D::Create(PathConfig::GetInstance().GetAssetsFolder().string() + "/Icons/PlayButton.png");
        m_StopButton = Texture2D::Create(PathConfig::GetInstance().GetAssetsFolder().string() + "/Icons/StopButton.png");

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

        // Update Runtime Camera. SHOULD BE OPTIMIZED BUT I DONT KNOW HOW TO DO THIS WITH CALLBACKS
        if (!IsEditorMode())
        {
            // m_RuntimeScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            UpdateRuntimeAspect();
        }

        // Framebuffer preparation
        m_Framebuffer->Bind();
        Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.75f });
        Renderer::Clear();

        // Clear entity ID attachment to -1
        // m_Framebuffer->ClearAttachment(1, -1);

		//NL_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

        if (IsEditorMode())
        {
            if (m_ViewportHovered || m_EditorCamera.IsMouseButtonHolding())
            {
                m_EditorCamera.OnUpdate(ts);
            }

            m_EditorScene->OnUpdateEditor(ts, m_EditorCamera);
        }
        else
        {
            if (!m_RuntimeCameraEntity.HasComponent<CameraComponent>())
            {
                auto camView = m_RuntimeScene->m_Registry.view<CameraComponent>();
                for (auto entity : camView)
                {
                    m_RuntimeCameraEntity = Entity(entity, m_RuntimeScene.get());
                    break;
                }
            }

            m_RuntimeScene->OnUpdateRuntime(ts, m_RuntimeCameraEntity);
        }

        // Check Hovered Entity

        if (IsEditorMode())
        {
            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_ViewportBounds[0].x;
            my -= m_ViewportBounds[0].y;
            nlm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
            my = viewportSize.y - my;
            int mouseX = (int)mx;
            int mouseY = (int)my;
            if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
            {
                int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
                m_EntityHovered = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_EditorScene.get());
                m_ViewportHovered = true;
            }
            else
            {
                m_ViewportHovered = false;
            }
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
                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Open", "Ctrl+O"))
                    OpenScene();

                if (ImGui::MenuItem("Save", "Ctrl+S"))
                    SaveScene();

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    SaveSceneAs();

                if (ImGui::MenuItem("Exit", "Alt + F4", false))
                    Application::GetInstance().Close();

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Viewport", NULL, &m_ShowViewport);
                ImGui::MenuItem("Hierarchy", NULL, &m_ShowHierarchy);
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
            ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollWithMouse |ImGuiWindowFlags_NoScrollbar);
            
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
                // Show Runtime Camera
                else if (m_ViewportMode == ViewportMode::Runtime)
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                    ImGui::PushItemWidth(menuBarWidth * 0.2f);

                    auto& runtimeCameraName = m_RuntimeCameraEntity.GetName();
                    if (ImGui::BeginCombo("Runtime Camera", runtimeCameraName.c_str()))
                    {
                        auto camView = m_RuntimeScene->m_Registry.view<CameraComponent>();
                        for (auto entity : camView)
                        {
                            auto& comp = m_RuntimeScene->m_Registry.get<CameraComponent>(entity);
                            auto& name = m_RuntimeScene->m_Registry.get<IdentityComponent>(entity).Name;
                            bool isSelected = entity == m_RuntimeCameraEntity;
                            if (ImGui::Selectable(name.c_str(), isSelected))
                            {
                                if (!isSelected)
                                {
                                    m_RuntimeCameraEntity = Entity(entity, m_RuntimeScene.get());
                                    // UpdateRuntimeAspect();
                                }
                            }
                        }

                        ImGui::EndCombo();
                    }

                    ImGui::PopStyleVar();
                    ImGui::PopItemWidth();
                }

                // Play & Stop Button
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

                ImGui::EndMenuBar();
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor(3);

            //

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


            // Grid
            if (IsEditorMode())
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
                const nlm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
                const nlm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
                ImGuizmo::DrawGrid(nlm::value_ptr(cameraView), nlm::value_ptr(cameraProjection), nlm::value_ptr(nlm::mat4(1.0f)), 20.0f);
            }
            
            m_ViewportFocused = ImGui::IsWindowFocused();
            // m_ViewportHovered = ImGui::IsWindowHovered();
            Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportHovered && !m_ViewportFocused);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            // Update Viewport Size
            m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

            // Update Viewport Image
            uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
            // uint64_t textureID = Library<Texture2D>::GetInstance().Get("../Assets/Models/nanosuit/arm_dif.png")->GetRendererID();
            if (!IsEditorMode())
            {
                ImGui::SetCursorPos(ImVec2{ ImGui::GetWindowContentRegionMin().x + (m_ViewportSize.x - m_RuntimeAspect.x) / 2.0f, ImGui::GetWindowContentRegionMin().y + (m_ViewportSize.y - m_RuntimeAspect.y) / 2.0f });
            }
            ImGui::Image(reinterpret_cast<void*>(textureID), IsEditorMode() ? ImVec2{m_ViewportSize.x, m_ViewportSize.y} : ImVec2{m_RuntimeAspect.x, m_RuntimeAspect.y}, ImVec2{0, 1}, ImVec2{1, 0});

            // Gizmos
            if (IsEditorMode())
            {
                Entity entitySelected = m_HierarchyPanel->GetSelectedEntity();
                if (entitySelected && entitySelected.HasComponent<TransformComponent>())
                {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();
                    ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                    // Editor camera
                    const nlm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
                    const nlm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

                    auto& component = entitySelected.GetComponent<TransformComponent>();
                    nlm::mat4 transform = component.GetTransform();

                    // Snap
                    bool snap = Input::IsKeyPressed(Key::LeftControl);
                    float snapValue = 0.5f; // Snap to 0.5m for translation/scale
                    // Snap to 45 degrees for rotation
                    if (m_GuizmoType == ImGuizmo::OPERATION::ROTATE)
                        snapValue = 45.0f;

                    float snapValues[3] = { snapValue, snapValue, snapValue };

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

            ImGui::End();
            ImGui::PopStyleVar();
        }

        // Hierarchy
        if (m_ShowHierarchy && !(m_IsMaximizeOnPlay && m_ViewportMode == ViewportMode::Runtime))
        {
            m_HierarchyPanel->OnImGuiRender(m_ShowHierarchy, true);
        }

#pragma endregion

        ImGui::End();

#pragma endregion

	}

	void EditorLayer::OnEvent(Event& event)
	{
        if (m_ViewportHovered || m_EditorCamera.IsMouseButtonHolding())
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
        if (event.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        bool mouseRight = Input::IsMouseButtonPressed(Mouse::ButtonRight);
        switch (event.GetKeyCode())
        {
        // Files
        case Key::N:
        {
            if (control)
                NewScene();
            break;
        }
        case Key::O:
        {
            if (control)
                OpenScene();
            break;
        }
        case Key::S:
        {
            if (control)
            {
                if (shift)
                    SaveSceneAs();
                else
                    SaveScene();
            }
            break;
        }

        // Focus
        case Key::F:
        {
            Entity selectedEntity = m_HierarchyPanel->GetSelectedEntity();
            if (selectedEntity)
            {
                // Ought to have transform...
                m_EditorCamera.SetCenter(selectedEntity.GetComponent<TransformComponent>().Translation);
            }
            break;
        }
        }

		return false;
	}

    bool EditorLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
    {
        if (event.GetMouseButton() == Mouse::ButtonLeft)
        {
            // !ImGuizmo::IsOver()
            if (m_ViewportHovered && !(ImGuizmo::IsOver() && m_TRSEntity == m_HierarchyPanel->GetSelectedEntity()))
            {
                m_HierarchyPanel->SetSelectedEntity(m_EntityHovered);
                // Click a viewport entity
                if (m_EntityHovered)
                {
                    NL_TRACE("Viewport select entity: {0}", m_EntityHovered.GetName());
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

    void EditorLayer::SerializeScene(Ref<Scene> scene, const std::string& path)
    {
        SceneSerializer serializer(scene);
        serializer.Serialize(path);
    }

    void EditorLayer::NewScene()
    {
        if (m_ViewportMode == ViewportMode::Runtime)
        {
            OnSceneStop();
        }

        if (m_EditorScene)
            m_EditorScene->m_Registry.clear();
        m_EditorScene = CreateRef<Scene>();
        m_HierarchyPanel->SetCurrentScene(m_EditorScene);
        m_EditorScenePath = "";

        Library<Texture2D>::GetInstance().TraverseDelete();
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

        Library<Texture2D>::GetInstance().TraverseDelete();
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
        if (serializer.Deserialize(path))
        {
            if (m_EditorScene)
                m_EditorScene->m_Registry.clear();

            m_EditorScene = newScene;            
            m_EditorScenePath = path;
            m_HierarchyPanel->SetCurrentScene(m_EditorScene);

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

            SerializeScene(m_EditorScene, path);
            m_EditorScenePath = path;

            Application::GetInstance().SetWindowTitle("Nameless Editor - " + path.substr(path.find_last_of("/\\") + 1));
        }
    }

    void EditorLayer::OnScenePlay()
    {
        m_ViewportMode = ViewportMode::Runtime;

        m_RuntimeScene = Scene::Copy(m_EditorScene);
        m_RuntimeScene->OnStartRuntime();

        m_HierarchyPanel->SetCurrentScene(m_RuntimeScene);
        // UpdateRuntimeAspect();
        // m_RuntimeScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        // m_ShowHierarchy = m_IsMaximizeOnPlay;
    }
    
    void EditorLayer::OnSceneStop()
    {
        m_ViewportMode = ViewportMode::Editor;

        m_RuntimeScene->OnStopRuntime();
        m_RuntimeScene.reset();
        m_RuntimeCameraEntity = {};

        m_HierarchyPanel->SetCurrentScene(m_EditorScene);
    }

    void EditorLayer::UpdateRuntimeAspect()
    {
        if (this->m_RuntimeCameraEntity.HasComponent<CameraComponent>())
        {
            auto& cam = this->m_RuntimeCameraEntity.GetComponent<CameraComponent>();
            if (cam.FixedAspectRatio)
            {
                this->m_RuntimeAspect = nlm::vec2(cam.mCamera.GetViewportWidth(), cam.mCamera.GetViewportHeight());
            }
            else
            {
                this->m_RuntimeAspect = nlm::vec2((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
                cam.mCamera.SetAspectRatio((uint32_t)this->m_ViewportSize.x, (uint32_t)this->m_ViewportSize.y);
            }
        }
    }
}