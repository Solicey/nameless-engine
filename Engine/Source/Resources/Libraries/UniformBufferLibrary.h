#pragma once

#include "Core/Math/Math.h"
#include "Renderer/UniformBuffer.h"
#include "Resources/Libraries/Library.h"

namespace NL
{
	// size: 152
	struct CameraData
	{
		nlm::mat4	ViewMatrix;
		nlm::mat4	ProjMatrix;
		nlm::vec3	CameraPos;
		float		Near;
		float		Far;
	};

	template <>
	class Library<UniformBuffer> : public LibraryBase<Library, UniformBuffer>
	{
	public:
		Library(Singleton::token)
		{
			Ref<UniformBuffer> CameraUniformBuffer = UniformBuffer::Create(148, 0);
			Add("CameraUniform", CameraUniformBuffer);

			Ref<UniformBuffer> ShadowUniformBuffer = UniformBuffer::Create(sizeof(nlm::mat4x4) * 16 + sizeof(nlm::vec4) * 16 + sizeof(nlm::vec3) + sizeof(float) * 2 + sizeof(int), 1);
			// lightSpaceMatrices * 16 + shadowCascadeLevels * 16 + lightDir + lightIntensity + farClip + cascadeLevel
			Add("ShadowUniform", ShadowUniformBuffer);
		}

		Ref<UniformBuffer> GetCameraUniformBuffer() const
		{
			return Get("CameraUniform");
		}

		Ref<UniformBuffer> GetShadowUniformBuffer() const
		{
			return Get("ShadowUniform");
		}
	};
}