#pragma once

#include "ECS/Component/Component.h"

#include "ECS/Component/Basic/TransformComponent.h"
#include "ECS/Component/Basic/IdentityComponent.h"
#include "ECS/Component/Render/ModelRendererComponent.h"

namespace NL
{
    template<Component... C>
    struct Components
    {

    };

    // Component to be copied
    using AllComponents = Components<TransformComponent, IdentityComponent, 
        ModelRendererComponent>;
}