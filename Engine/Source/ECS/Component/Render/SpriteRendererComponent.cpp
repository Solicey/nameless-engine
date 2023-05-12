#include "nlpch.h"
#include "SpriteRendererComponent.h"

#include "Resources/Libraries/TextureLibrary.h"

#include <regex>

namespace NL
{
	SpriteRendererComponent::SpriteRendererComponent(const std::string& texPath, const nlm::vec4& color, SpriteCameraReaction reaction)
		: SpriteTexture(Library<Texture2D>::GetInstance().Fetch(texPath)), Color(color), Reaction(reaction)
	{
		if (SpriteTexture != nullptr)
			Path = std::regex_replace(texPath, std::regex("\\\\"), "/");
	}

	void SpriteRendererComponent::ReplaceTexture(const std::string& texPath)
	{
		if (Path == std::regex_replace(texPath, std::regex("\\\\"), "/") && SpriteTexture != nullptr)
			return;
		SpriteTexture.reset();
		NL_ENGINE_INFO("Replace Sprite: {0}", Path);
		Library<Texture2D>::GetInstance().Delete(Path);
		SpriteTexture = Library<Texture2D>::GetInstance().Fetch(texPath);
		Path = texPath;
	}
}
