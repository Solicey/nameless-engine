#include "nlpch.h"

#include "Scene.h"

#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"
#include "ECS/System/AllSystems.h"
#include "Scripting/ScriptEngine.h"

#include "Resources/Libraries/TextureLibrary.h"
#include "Resources/Libraries/MeshLibrary.h"

namespace NL
{
    Scene::Scene()
    {
        m_Systems.emplace_back(CreateScope<ScriptingSystem>(this));
        m_Systems.emplace_back(CreateScope<RenderSystem>(this));
        m_EntityMap.clear();
        m_SkyboxTextures.resize(6);
        const std::string& name = Library<Texture2D>::GetInstance().GetDefaultTextureName();
        for (int i = 0; i < 6; i++)
        {
            m_SkyboxTextures[i] = name;
        }
    }

    Scene::~Scene()
    {
        // DestroyScene();
    }

    template<Component... C>
    static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<ID, entt::entity>& enttMap)
    {
        ([&]()
        {
        
        auto view = src.view<C>();
        for (auto srcEntity : view)
        {
            entt::entity dstEntity = enttMap.at(src.get<IdentityComponent>(srcEntity).ID);

            auto& srcComponent = src.get<C>(srcEntity);
            dst.emplace_or_replace<C>(dstEntity, srcComponent);
        }
        
        }(), ...);
    }

    template<Component... C>
    static void CopyComponent(Components<C...>, entt::registry& dst, entt::registry& src, const std::unordered_map<ID, entt::entity>& enttMap)
    {
        CopyComponent<C...>(dst, src, enttMap);
    }

    template<Component... C>
    static void CopyComponentIfExists(Entity dst, Entity src)
    {
        ([&]()
            {
                if (src.HasComponent<C>())
                    dst.AddOrReplaceComponent<C>(src.GetComponent<C>());
            }(), ...);
    }

    template<Component... C>
    static void CopyComponentIfExists(Components<C...>, Entity dst, Entity src)
    {
        CopyComponentIfExists<C...>(dst, src);
    }

    Ref<Scene> Scene::DuplicateScene(Ref<Scene> scene)
    {
        NL_ENGINE_TRACE("Ready to copy.");

        Ref<Scene> newScene = CreateRef<Scene>();

        std::unordered_map<ID, entt::entity> enttMap;

        auto& srcSceneRegistry = scene->Registry;
        auto& dstSceneRegistry = newScene->Registry;
        auto idView = srcSceneRegistry.view<IdentityComponent>();
        for (auto entity : idView)
        {
            auto& comp = srcSceneRegistry.get<IdentityComponent>(entity);
            ID id = comp.ID;
            const auto& name = comp.Name;
            Entity newEntity = newScene->CreateEntityWithID(id, name);
            enttMap[id] = newEntity;
        }

        // DuplicateScene components (except IdentityComponent)
        CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

        return newScene;
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithID(ID(), name);
    }

    Entity Scene::CreateEntityWithID(ID id, const std::string& name)
    {
        Entity entity = Entity(Registry.create(), this);
        NL_ENGINE_TRACE("Entity entt id: {0}, nl id: {1}", (uint32_t)entity, id.GetID());
        entity.AddComponent<IdentityComponent>(id, name);
        entity.AddComponent<TransformComponent>();

        m_EntityMap[id] = entity;

        return entity;
    }

    Entity Scene::DuplicateEntity(Entity entity)
    {
        Entity newEntity = CreateEntity(entity.GetName());
        CopyComponentIfExists(AllComponents{}, newEntity, entity);

        NL_ENGINE_INFO("New Entity ID: {0}, prototype ID: {1}", newEntity.GetID(), entity.GetID());
        return newEntity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        if (entity.HasComponent<ModelRendererComponent>())
        {
            auto& model = entity.GetComponent<ModelRendererComponent>()._Model;
            if (model)
                model->DeleteMeshesAndMaterialsReference();
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            auto& comp = entity.GetComponent<SpriteRendererComponent>();
            comp.SpriteTexture.reset();
            Library<Texture2D>::GetInstance().Delete(comp.Path);
        }

        if (entity.HasComponent<ParticleSystemComponent>())
        {
            auto& comp = entity.GetComponent<ParticleSystemComponent>();
            comp.DeleteMaterialsReference();
        }

        if (entity.HasComponent<PostProcessingComponent>())
        {
            auto& comp = entity.GetComponent<PostProcessingComponent>();
            comp.DeleteMaterialsReference();
        }

        m_EntityMap.erase(entity.GetID());
        Registry.destroy(entity);
    }

    void Scene::DestroyScene()
    {
        auto view = Registry.view<IdentityComponent>();
        for (auto entity : view)
        {
            DestroyEntity(Entity(entity, this));
        }

        Registry.clear();
        m_EntityMap.clear();
        //Library<Texture2D>::GetInstance().TraverseDelete();
        //Library<Mesh>::GetInstance().TraverseDelete();
    }

    void Scene::OnStartRuntime()
    {
        m_SceneState = SceneState::Play;
        for (auto& system : m_Systems)
        {
            system->OnStartRuntime();
        }
    }

    void Scene::OnStopRuntime(Scene* editorScene)
    {
        //GetSettingsEntity().GetComponent<SettingsComponent>().EditorCamera = nullptr;
        m_SceneState = SceneState::Editor;
        for (auto& system : m_Systems)
        {
            system->OnStopRuntime(editorScene);
        }
    }

    void Scene::OnUpdateRuntime(TimeStep ts, bool isRuntimeViewportFocused)
    {
        if (!isRuntimeViewportFocused)
            m_SceneState = SceneState::Pause;
        else m_SceneState = SceneState::Play;

        for (auto& system : m_Systems)
        {
            system->OnUpdateRuntime(ts);
        }
    }

    void Scene::OnStartEditor()
    {
        for (auto& system : m_Systems)
        {
            system->OnStartEditor();
        }
    }

    void Scene::OnUpdateEditor(TimeStep ts)
    {
        for (auto& system : m_Systems)
        {
            system->OnUpdateEditor(ts);
        }
    }

    Entity Scene::GetEntityWithID(ID id)
    {
        if (m_EntityMap.find(id) != m_EntityMap.end())
            return { m_EntityMap.at(id), this };

        return {};
    }

    Entity Scene::FindEntityByName(std::string_view name)
    {
        auto view = Registry.view<IdentityComponent>();
        for (auto entity : view)
        {
            const IdentityComponent& comp = view.get<IdentityComponent>(entity);
            if (comp.Name == name)
                return Entity{ entity, this };
        }
        return {};
    }

    Entity Scene::GetSettingsEntity()
    {
        auto view = Registry.view<SettingsComponent>();
        for (auto entity : view)
        {
            return Entity{ entity, this };
        }
        return {};
    }

    void Scene::PackUpLightDatas()
    {
        m_PointLightShadingDatas.clear();
        m_DirLightShadingDatas.clear();

        auto view = Registry.view<TransformComponent, LightComponent>();
        int pointId = 0, dirId = 0;
        for (auto& e : view)
        {
            Entity entity = Entity(e, this);
            auto [transform, light] = view.get<TransformComponent, LightComponent>(e);

            if (light.Type == LightType::Point)
            {
                nlm::vec3 color = light.Color * light.Intensity;

                PointLightShadingData info = {
                    transform.GetTranslation(),
                    color,
                    light.Attenuation,
                    (uint32_t)entity
                };
                m_PointLightShadingDatas.push_back(info);
            }
            else if (light.Type == LightType::Directional)
            {
                nlm::vec3 color = light.Color * light.Intensity;

                DirLightShadingData info = {
                    transform.GetTranslation(),
                    transform.GetForward(),
                    color,
                    (uint32_t)entity
                };
                m_DirLightShadingDatas.push_back(info);
            }
        }
    }

    /*nlm::vec2 Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        auto view = Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
                cameraComponent.mCamera.SetAspectRatio(width, height);
        }
    }*/

#pragma region OnComponentAdded

    template<Component C>
    void Scene::OnComponentAdded(Entity entity, C& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<IdentityComponent>(Entity entity, IdentityComponent& component)
    {
    }

    template<>
    void Scene::OnComponentAdded<ModelRendererComponent>(Entity entity, ModelRendererComponent& component)
    {
        NL_ENGINE_TRACE("Entity {0}: Added ModelRendererComponent!", entity.GetName());
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        NL_ENGINE_TRACE("Entity {0}: Added CameraComponent!", entity.GetName());

        component.LoadGizmosModel((uint32_t)entity);
        // m_RuntimeCameraMap.insert(std::make_pair(entity, component.mCamera));
    }

    template<>
    void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
    {

    } 
    
    template<>
    void Scene::OnComponentAdded<SettingsComponent>(Entity entity, SettingsComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<ParticleSystemComponent>(Entity entity, ParticleSystemComponent& component)
    {
        NL_ENGINE_INFO("Add Particle System!");
    }

    template<>
    void Scene::OnComponentAdded<PostProcessingComponent>(Entity entity, PostProcessingComponent& component)
    {
        NL_ENGINE_INFO("Add Post Processing System!");
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
        NL_ENGINE_INFO("Add Sprite Renderer Component!");
    }

#pragma endregion

#pragma region OnComponentRemoved

    template<Component C>
    void Scene::OnComponentRemoved(Entity entity, C& component)
    {

    }

    template<>
    void Scene::OnComponentRemoved<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<IdentityComponent>(Entity entity, IdentityComponent& component)
    {
    }

    template<>
    void Scene::OnComponentRemoved<ModelRendererComponent>(Entity entity, ModelRendererComponent& component)
    {
        if (component._Model)
        {
            component._Model->DeleteMeshesAndMaterialsReference();
        }
    }

    template<>
    void Scene::OnComponentRemoved<CameraComponent>(Entity entity, CameraComponent& component)
    {
        // m_RuntimeCameraMap.erase(entity);
    }

    template<>
    void Scene::OnComponentRemoved<ScriptComponent>(Entity entity, ScriptComponent& component)
    {
       
    }

    template<>
    void Scene::OnComponentRemoved<LightComponent>(Entity entity, LightComponent& component)
    {

    }

    template<>
    void Scene::OnComponentRemoved<ParticleSystemComponent>(Entity entity, ParticleSystemComponent& component)
    {
        component.DeleteMaterialsReference();
    }

    template<>
    void Scene::OnComponentRemoved<PostProcessingComponent>(Entity entity, PostProcessingComponent& component)
    {
        component.DeleteMaterialsReference();
    }

    template<>
    void Scene::OnComponentRemoved<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {
        if (component.SpriteTexture != nullptr)
        {
            component.SpriteTexture.reset();
            Library<Texture2D>::GetInstance().Delete(component.Path);
        }
    }

    template<>
    void Scene::OnComponentRemoved<SettingsComponent>(Entity entity, SettingsComponent& component)
    {

    }


#pragma endregion
}