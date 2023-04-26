#pragma once

#include "Core/Misc/PtrWrapper.h"
#include "Renderer/Alternatives.h"

#include <cstdint>
#include <filesystem>

namespace NL
{
	enum class TextureType
	{
		Diffuse,
		Specular,
		Ambient,
		Emissive,
		Height,
		Normals,
		Shininess,
		Opacity,
		Displacement,
		Lightmap,

		// PBR
		Metalness,
		Roughness,

		CubeMap
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		// virtual const std::string& GetPath() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;

		virtual bool IsLoaded() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		template <typename LibType>
		friend class Library;

		virtual const std::string& GetPath() const = 0;

	private:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
	};

	enum class CubeMapFace : uint32_t
	{
		Right = 0,
		Left = 1,
		Top = 2,
		Bottom = 3,
		Front = 4,
		Back = 5,
	};

	class TextureCubeMap : public Texture
	{
	public:
		template <typename LibType>
		friend class Library;
		// virtual void AddTexturePath(CubeMapFace faceIndex, const std::string& path) = 0;

	private:
		static Ref<TextureCubeMap> Create(const std::vector<std::string>& texturePaths);
	};
}