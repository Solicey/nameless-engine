#pragma once

#include "Core/Math/Math.h"
#include "Core/Misc/PtrWrapper.h"
#include "ECS/Component/Component.h"
#include "Resources/Texture.h"
#include "Renderer/Renderer.h"

namespace NL
{
	class SpriteRendererComponent : public ComponentBase
	{
	public:
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const std::string& texPath, const nlm::vec4& color = nlm::vec4(1.0f), SpriteCameraReaction reaction = SpriteCameraReaction::Normal);

		void ReplaceTexture(const std::string& texPath);

	public:
		std::string Path = "";
		Ref<Texture2D> SpriteTexture;
		nlm::vec4 Color = nlm::vec4(1.0f);
		SpriteCameraReaction Reaction;
	};
}