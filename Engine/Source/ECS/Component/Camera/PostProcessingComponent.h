#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "ECS/Component/Component.h"
#include "Resources/Material.h"

namespace NL
{
	class PostProcessingComponent : public ComponentBase
	{
	public:
		PostProcessingComponent() = default;
		PostProcessingComponent(const PostProcessingComponent& comp);
		void DeleteMaterialsReference();
		void UpdateShaderProperties(const std::string& shaderName);

	public:
		std::vector<Ref<Material>> Queue;
	};
}