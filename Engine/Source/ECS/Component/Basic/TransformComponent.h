#pragma once

#include "ECS/Component/Component.h"
#include "Core/Math/Math.h"

namespace NL
{
    class TransformComponent : public ComponentBase
    {
    public:
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const nlm::vec3& translation)
            : Translation(translation) {}
        TransformComponent(const nlm::vec3& translation, const nlm::vec3& rotation, const nlm::vec3& scale)
            : Translation(translation), Rotation(rotation), Scale(scale) {}

        nlm::mat4 GetTransform() const
        {
            return nlm::translate(nlm::mat4(1.0f), Translation) * GetRotationMatrix() * nlm::scale(nlm::mat4(1.0f), Scale);
        }

        nlm::mat4 GetRotationMatrix() const
        {
            return nlm::toMat4(nlm::quat(Rotation));
        }

        nlm::vec3 GetTranslation() const { return Translation; }
        nlm::vec3 GetRotation() const { return Rotation; }
        nlm::vec3 GetScale() const { return Scale; }

        void SetTranslation(float x, float y, float z) { Translation = { x, y, z }; }
        void SetRotation(float x, float y, float z) { Rotation = { x, y, z }; }
        void SetScale(float x, float y, float z) { Scale = { x, y, z }; }

        nlm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        nlm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };  // Euler angles
        nlm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
    };
}