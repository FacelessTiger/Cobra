#pragma once

#include <Cobra/Asset/AssetMetadata.h>

namespace Cobra {

	class AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);
	};

}