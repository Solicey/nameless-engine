#pragma once

#include "Core/Math/Math.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

namespace NL
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static bool IsMouseButtonReleased(MouseCode button);
        static nlm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}