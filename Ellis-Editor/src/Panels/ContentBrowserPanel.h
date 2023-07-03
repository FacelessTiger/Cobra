#pragma once

#include <filesystem>
#include <map>
#include <unordered_map>
#include <set>

#include "Ellis/Renderer/Texture.h"

namespace Ellis {

	class ContentBrowserPanel
	{
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;

		struct TreeNode
		{
			std::filesystem::path Path;
			AssetHandle Handle = 0;

			uint32_t Parent = (uint32_t)-1;
			std::map<std::filesystem::path, uint32_t> Children;

			TreeNode(const std::filesystem::path& path, AssetHandle handle)
				: Path(path), Handle(handle) { }
		};

		std::vector<TreeNode> m_TreeNodes;

		enum class Mode
		{
			Asset = 0, FileSystem
		};

		Mode m_Mode = Mode::FileSystem;
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	private:
		void RefreshAssetTree();
	};

}