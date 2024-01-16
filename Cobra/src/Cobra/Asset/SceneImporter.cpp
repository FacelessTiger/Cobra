#include "cbpch.h"
#include "SceneImporter.h"

#include <Cobra/Project/Project.h>
#include <Cobra/Scene/SceneSerializer.h>

namespace Cobra {

	Ref<Scene> SceneImporter::ImportScene(AssetHandle handle, const AssetMetadata& metadata)
	{
		CB_PROFILE_FUNCTION();

		return LoadScene(Project::GetActiveAssetDirectory() / metadata.FilePath);
	}

	Ref<Scene> SceneImporter::LoadScene(const std::filesystem::path& path)
	{
		CB_PROFILE_FUNCTION();

		Ref<Scene> scene = CreateRef<Scene>();

		SceneSerializer serializer(scene);
		serializer.Deserialize(path);
		return scene;
	}

	void SceneImporter::SaveScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(Project::GetActiveAssetDirectory() / path);
	}

}