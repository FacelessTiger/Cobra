#pragma once

#include <Cobra/Asset/AssetManagerBase.h>
#include <Cobra/Project/Project.h>

namespace Cobra {

	class AssetManager
	{
	public:
		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			Ref<Asset> asset = Project::GetActive()->GetAssetManager()->GetAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}

		static bool IsAssetHandleValid(AssetHandle handle);
		static bool IsAssetLoaded(AssetHandle handle);
		static AssetType GetAssetType(AssetHandle handle);
	};

}