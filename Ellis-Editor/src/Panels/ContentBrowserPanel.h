#pragma once

#include <filesystem>
#include <unordered_map>

#include "Ellis/Renderer/Texture.h"

namespace Ellis {

	class ContentBrowserPanel
	{
	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		std::unordered_map<std::string, Ref<Texture2D>> m_ImageIcons;
	public:
		ContentBrowserPanel();

		void OnImGuiRender();
	};

}