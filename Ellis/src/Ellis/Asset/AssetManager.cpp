#include "elpch.h"
#include "AssetManager.h"

namespace Ellis {

    bool AssetManager::IsAssetHandleValid(AssetHandle handle)
    {
        return Project::GetActive()->GetAssetManager()->IsAssetHandleValid(handle);
    }

    bool AssetManager::IsAssetLoaded(AssetHandle handle)
    {
        return Project::GetActive()->GetAssetManager()->IsAssetLoaded(handle);
    }

    AssetType AssetManager::GetAssetType(AssetHandle handle)
    {
        return Project::GetActive()->GetAssetManager()->GetAssetType(handle);
    }

}