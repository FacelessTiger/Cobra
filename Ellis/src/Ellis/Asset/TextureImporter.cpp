#include "elpch.h"
#include "TextureImporter.h"

#include <Ellis/Project/Project.h>

#include <stb_image.h>

namespace Ellis {

	Ref<Texture2D> TextureImporter::ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata)
	{
		EL_PROFILE_FUNCTION();

		return LoadTexture2D(Project::GetAssetDirectory() / metadata.FilePath);
	}

	Ref<Texture2D> TextureImporter::LoadTexture2D(const std::filesystem::path& path)
	{
		EL_PROFILE_FUNCTION();

		int width, height, channels;
		Buffer data;

		stbi_set_flip_vertically_on_load(1);
		{
			EL_PROFILE_SCOPE("stbi_load - TextureImporter::ImportTexture2D");
			std::string pathStr = path.string();

			data.Data = stbi_load(pathStr.c_str(), &width, &height, &channels, 0);
		}

		if (!data)
		{
			EL_CORE_ERROR("TextureImporter::ImportTexture2D - Could not load texture from filepath: {}", path.string());
			return nullptr;
		}

		data.Size = width * height * channels;

		TextureSpecification spec;
		spec.Width = width;
		spec.Height = height;

		switch (channels)
			{
			case 3: spec.Format = ImageFormat::RGB8; break;
			case 4: spec.Format = ImageFormat::RGBA8; break;
		}

		Ref<Texture2D> texture = Texture2D::Create(spec, data);
		data.Release();
		return texture;
	}

}