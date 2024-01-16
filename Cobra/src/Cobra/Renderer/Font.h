#pragma once

#include <filesystem>

#include <Cobra/Renderer/Texture.h>

namespace Cobra {

	struct MSDFData;

	class Font
	{
	private:
		MSDFData* m_Data = nullptr;
		Ref<Texture2D> m_AtlasTexture;
	public:
		Font(const std::filesystem::path& filepath);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();
	};

}