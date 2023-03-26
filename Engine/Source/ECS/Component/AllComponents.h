#pragma once

#include "ECS/Component/Component.h"

#include "ECS/Component/Basic/TransformComponent.h"
#include "ECS/Component/Basic/IdentityComponent.h"
#include "ECS/Component/Render/ModelRendererComponent.h"
#include "ECS/Component/Camera/CameraComponent.h"
#include "ECS/Component/Scripting/ScriptComponent.h"

namespace NL
{
    template<Component... C>
    struct Components
    {

    };

    // Component to be copied
    using AllComponents = Components<TransformComponent, CameraComponent, ModelRendererComponent, ScriptComponent>;
}