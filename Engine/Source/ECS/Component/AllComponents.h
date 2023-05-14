#pragma once

#include "ECS/Component/Component.h"

#include "ECS/Component/Basic/TransformComponent.h"
#include "ECS/Component/Basic/IdentityComponent.h"
#include "ECS/Component/Basic/SettingsComponent.h"
#include "ECS/Component/Render/ModelRendererComponent.h"
#include "ECS/Component/Camera/CameraComponent.h"
#include "ECS/Component/Camera/PostProcessingComponent.h"
#include "ECS/Component/Scripting/ScriptComponent.h"
#include "ECS/Component/Light/LightComponent.h"
#include "ECS/Component/VFX/ParticleSystemComponent.h"
#include "ECS/Component/Render/SpriteRendererComponent.h"

namespace NL
{
    template<Component... C>
    struct Components
    {

    };

    // Component to be copied
    using AllComponents = Components<TransformComponent, CameraComponent, PostProcessingComponent, ModelRendererComponent, ScriptComponent, LightComponent, ParticleSystemComponent, SpriteRendererComponent, SettingsComponent>;
}