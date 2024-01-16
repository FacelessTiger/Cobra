#pragma once

#include <Cobra/Core/Core.h>

#include <Cobra/Asset/RuntimeAssetManager.h>
#include <Cobra/Asset/EditorAssetManager.h>

#include <string>
#include <filesystem>

namespace Cobra {

	struct ProjectConfig
	{
		std::string Name = "Untitled";

		AssetHandle StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetRegistryPath;
		std::filesystem::path ScriptModulePath;
	};

	class Project
	{
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		std::shared_ptr<AssetManagerBase> m_AssetManager;

		inline static Ref<Project> s_ActiveProject;
	public:
		const std::filesystem::path& GetProjectDirectory() { return m_ProjectDirectory; }

		std::filesystem::path GetAssetDirectory() { return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory; }
		std::filesystem::path GetAssetRegistryPath() { return GetAssetDirectory() / s_ActiveProject->m_Config.AssetRegistryPath; }
		// TODO: move to asset manager when we have one
		std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) { return GetAssetDirectory() / path; }

		std::filesystem::path GetAssetAbsolutePath(const std::filesystem::path& path);

		static const std::filesystem::path& GetActiveProjectDirectory()
		{
			CB_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetProjectDirectory();
		}

		static std::filesystem::path GetActiveAssetDirectory()
		{
			CB_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetAssetDirectory();
		}

		static std::filesystem::path GetActiveAssetRegistryPath()
		{
			CB_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetAssetRegistryPath();
		}

		static std::filesystem::path GetActiveAssetFileSystemPath(const std::filesystem::path& path)
		{
			CB_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetAssetFileSystemPath(path);
		}

		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }
		std::shared_ptr<AssetManagerBase> GetAssetManager() { return m_AssetManager; }
		std::shared_ptr<RuntimeAssetManager> GetRuntimeAssetManager() { return std::static_pointer_cast<RuntimeAssetManager>(m_AssetManager); }
		std::shared_ptr<EditorAssetManager> GetEditorAssetManager() { return std::static_pointer_cast<EditorAssetManager>(m_AssetManager); }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);
	};

}