#pragma once

#include <Ellis/Asset/AssetMetadata.h>
#include <Ellis/Renderer/Texture.h>

namespace Ellis {

	class TextureImporter
	{
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);

		// Reads file directly from filesystem (i.e. path has to be relative/absolute to working directory)
		static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path);
	};

}