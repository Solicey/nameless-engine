#pragma once

#include "Core/Misc/PtrWrapper.h"

namespace NL
{
    class GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static Ref<GraphicsContext> Create(void* window);
    };
}