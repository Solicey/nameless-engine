#include "nlpch.h"

#include "Scene.h"

#include "ECS/Entity/Entity.h"
#include "ECS/Component/AllComponents.h"
#include "ECS/System/AllSystems.h"

namespace NL
{
    Scene::Scene()
    {
        m_Systems.emplace_back(CreateScope<RenderSystem>(this));
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

    Ref<Scene> Scene::Copy(Ref<Scene> scene)
    {
        NL_ENGINE_TRACE("Ready to copy.");

        Ref<Scene> newScene = CreateRef<Scene>();

        std::unordered_map<ID, entt::entity> enttMap;

        auto& srcSceneRegistry = scene->m_Registry;
        auto& dstSceneRegistry = newScene->m_Registry;
        auto idView = srcSceneRegistry.view<IdentityComponent>();
        for (auto entity : idView)
        {
            auto& comp = srcSceneRegistry.get<IdentityComponent>(entity);
            ID id = comp.ID;
            const auto& name = comp.Name;
            Entity newEntity = newScene->CreateEntityWithID(id, name);
            enttMap[id] = newEntity;
        }

        // Copy components (except IdentityComponent)
        CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

        return newScene;
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithID(ID(), name);
    }

    Entity Scene::CreateEntityWithID(ID id, const std::string& name)
    {
        Entity entity = Entity(m_Registry.create(), this);
        NL_ENGINE_TRACE("Entity entt id: {0}, nl id: {1}", (uint32_t)entity, id.GetID());
        entity.AddComponent<IdentityComponent>(id, name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        if (entity.HasComponent<ModelRendererComponent>())
        {
            auto& model = entity.GetComponent<ModelRendererComponent>().mModel;
            if (model)
                model->DeleteMaterialTexturesReference();
        }

        m_Registry.destroy(entity);
    }

    void Scene::OnStartRuntime()
    {
        for (auto& system : m_Systems)
        {
            system->OnStartRuntime();
        }
    }

    void Scene::OnStopRuntime()
    {
        for (auto& system : m_Systems)
        {
            system->OnStopRuntime();
        }
    }

    void Scene::OnUpdateRuntime(TimeStep ts, Entity cameraEntity)
    {
        for (auto& system : m_Systems)
        {
            system->OnUpdateRuntime(ts, cameraEntity);
        }
    }

    void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
    {
        for (auto& system : m_Systems)
        {
            system->OnUpdateEditor(ts, camera);
        }
    }

    /*nlm::vec2 Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        auto view = m_Registry.view<CameraComponent>();
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
    void Scene::OnComponentAdded<ScriptingComponent>(Entity entity, ScriptingComponent& component)
    {
        
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
        if (component.mModel)
        {
            component.mModel->DeleteMaterialTexturesReference();
        }
    }

    template<>
    void Scene::OnComponentRemoved<CameraComponent>(Entity entity, CameraComponent& component)
    {
        // m_RuntimeCameraMap.erase(entity);
    }

    template<>
    void Scene::OnComponentRemoved<ScriptingComponent>(Entity entity, ScriptingComponent& component)
    {
       
    }

#pragma endregion
}