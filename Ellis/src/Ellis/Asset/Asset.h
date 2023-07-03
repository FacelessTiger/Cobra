#pragma once

#include <Ellis/Core/UUID.h>

namespace Ellis {

	using AssetHandle = UUID;

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Texture2D
	};

	std::string_view AssetTypeToString(AssetType type);
	AssetType AssetTypeFromString(std::string_view type);

	class Asset
	{
	public:
		AssetHandle Handle;

		virtual AssetType GetType() const = 0;
	};

}