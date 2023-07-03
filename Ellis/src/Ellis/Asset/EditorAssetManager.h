#pragma once

#include <Ellis/Asset/AssetManagerBase.h>
#include <Ellis/Asset/AssetMetadata.h>

namespace Ellis {

	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

	class EditorAssetManager : public AssetManagerBase
	{
	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;
	public:
		Ref<Asset> GetAsset(AssetHandle handle) override;

		bool IsAssetHandleValid(AssetHandle handle) const override;
		bool IsAssetLoaded(AssetHandle handle) const override;
		AssetType GetAssetType(AssetHandle handle) const override;

		void ImportAsset(const std::filesystem::path& filepath);

		void SerializeAssetRegistry();
		bool DeserializeAssetRegistry();

		const AssetMetadata& GetMetadata(AssetHandle handle) const;
		const std::filesystem::path& GetFilePath(AssetHandle handle) const;
		const AssetRegistry& GetAssetRegistry() const { return m_AssetRegistry; }
	};

}