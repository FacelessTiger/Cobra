#pragma once

#include <Ellis/Asset/AssetMetadata.h>

namespace Ellis {

	class AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);
	};

}