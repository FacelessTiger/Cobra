#pragma once

#include <Cobra/Asset/Asset.h>

#include <filesystem>

namespace Cobra {

	struct AssetMetadata
	{
		AssetType Type = AssetType::None;
		std::filesystem::path FilePath;

		operator bool() const { return Type != AssetType::None; }
	};

}