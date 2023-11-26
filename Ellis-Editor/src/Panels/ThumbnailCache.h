#pragma once

#include <Ellis/Project/Project.h>
#include <Ellis/Renderer/Texture.h>

#include <queue>

namespace Ellis {

	struct ThumbnailImage
	{
		uint64_t Timestamp;
		Ref<Texture2D> Image;
	};

	class ThumbnailCache
	{
	public:
		ThumbnailCache(Ref<Project> project);

		Ref<Texture2D> GetOrCreateThumbnail(const std::filesystem::path& assetPath);
		void OnUpdate();
	private:
		Ref<Project> m_Project;
		std::map<std::filesystem::path, ThumbnailImage> m_CachedImages;

		struct ThumbnailInfo
		{
			std::filesystem::path AbsolutePath;
			std::filesystem::path AssetPath;
			uint64_t Timestamp;
		};
		std::queue<ThumbnailInfo> m_Queue;

		// TEMP (replace with Ellis::Serialization)
		std::filesystem::path m_ThumbnailCachePath;
	};

}