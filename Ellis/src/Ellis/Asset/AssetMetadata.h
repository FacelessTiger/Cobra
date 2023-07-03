#pragma once

#include <Ellis/Asset/Asset.h>

#include <filesystem>

namespace Ellis {

	struct AssetMetadata
	{
		AssetType Type = AssetType::None;
		std::filesystem::path FilePath;

		operator bool() const { return Type != AssetType::None; }
	};

}