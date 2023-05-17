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
			Ref<UniformBuffer>	CameraUniformBuffer = UniformBuffer::Create(148, 0);
			Add("CameraUniform", CameraUniformBuffer);
		}

		Ref<UniformBuffer> GetCameraUniformBuffer() const
		{
			return Get("CameraUniform");
		}
	};
}