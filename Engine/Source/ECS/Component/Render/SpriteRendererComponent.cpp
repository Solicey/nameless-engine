#include "nlpch.h"
#include "SpriteRendererComponent.h"

#include "Resources/Libraries/TextureLibrary.h"

#include <regex>

namespace NL
{
	SpriteRendererComponent::SpriteRendererComponent(const std::string& texPath, const nlm::vec4& color)
		: SpriteTexture(Library<Texture2D>::GetInstance().Fetch(texPath)), Color(color)
	{
		if (SpriteTexture != nullptr)
			Path = std::regex_replace(texPath, std::regex("\\\\"), "/");
	}

	void SpriteRendererComponent::ReplaceTexture(const std::string& texPath)
	{
		if (Path == std::regex_replace(texPath, std::regex("\\\\"), "/"))
			return;
		SpriteTexture.reset();
		NL_ENGINE_INFO("Replace Sprite: {0}", Path);
		Library<Texture2D>::GetInstance().Delete(Path);
		SpriteTexture = Library<Texture2D>::GetInstance().Fetch(texPath);
		Path = texPath;
	}
}
