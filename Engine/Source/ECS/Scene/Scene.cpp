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

    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntityWithID(ID(), name);
    }

    Entity Scene::CreateEntityWithID(ID id, const std::string& name)
    {
        Entity entity = Entity(m_Registry.create(), this);
        entity.AddComponent<IdentityComponent>(id, name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
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

    void Scene::OnUpdateRuntime(TimeStep ts)
    {
        for (auto& system : m_Systems)
        {
            system->OnUpdateRuntime(ts);
        }
    }

    void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
    {
        for (auto& system : m_Systems)
        {
            system->OnUpdateEditor(ts, camera);
        }
    }

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
}