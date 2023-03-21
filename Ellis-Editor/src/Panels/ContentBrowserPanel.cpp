#include "elpch.h"
#include "ContentBrowserPanel.h"

#include "Ellis/Project/Project.h"

#include <imgui/imgui.h>

namespace Ellis {

	ContentBrowserPanel::ContentBrowserPanel()
		: m_BaseDirectory(Project::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{ 
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != m_BaseDirectory)
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 96;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		// Make sure cached texture icons exist, if they dont remove them from cache
		for (auto it = m_ImageIcons.cbegin(), next_it = it; it != m_ImageIcons.cend(); it = next_it)
		{
			++next_it;

			if (!std::filesystem::exists((*it).first))
				m_ImageIcons.erase(it);
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			std::unordered_set<std::string> imageExtensions = { ".png", ".jpg" };

			if (imageExtensions.find(path.extension().string()) != imageExtensions.end())
			{
				if (m_ImageIcons.find(path.string()) == m_ImageIcons.end())
					m_ImageIcons[path.string()] = Texture2D::Create(path.string());

				icon = m_ImageIcons[path.string()];
			}

			ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), {thumbnailSize, thumbnailSize}, { 0, 1}, { 1, 0 });
			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();

				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}

			ImGui::TextWrapped(filenameString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}

}