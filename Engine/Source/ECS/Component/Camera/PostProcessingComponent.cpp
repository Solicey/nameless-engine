#include "nlpch.h"
#include "PostProcessingComponent.h"

namespace NL
{
	PostProcessingComponent::PostProcessingComponent(const PostProcessingComponent& comp)
	{
		Queue.clear();
		for (auto& mat : comp.Queue)
		{
			Queue.push_back(CreateRef<Material>(mat.get()));
		}
	}

	void PostProcessingComponent::DeleteMaterialsReference()
	{
		for (auto& mat : Queue)
		{
			mat->DeleteTexturesAndShadersReference();
		}
		Queue.clear();
	}

	void PostProcessingComponent::UpdateShaderProperties(const std::string& shaderName)
	{
		for (auto& mat : Queue)
		{
			if (mat->GetShaderName() == shaderName)
				mat->LoadShaderAndUpdateProps(shaderName);
		}
	}
}
