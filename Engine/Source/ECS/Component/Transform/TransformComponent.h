#pragma once

#include "ECS/Component/Components.h"

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

		// ×¢ÒâË³Ðò
		const nlm::mat4& GetTransformMatrix() const
		{
			return nlm::translate(nlm::mat4(1.0f), Translation) * GetRotationMatrix() * nlm::scale(nlm::mat4(1.0f), Scale);
		}
		
		const nlm::mat4& GetRotationMatrix() const
		{
			return nlm::toMat4(nlm::quat(Rotation));
		}

		const nlm::vec3& GetTranslation() const { return Translation; }

		void SetTranslation(float x, float y, float z) { Translation = { x, y, z }; }

		nlm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		nlm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		nlm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
	};
}