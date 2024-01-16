#pragma once

#include <Cobra/Core/Buffer.h>

namespace Cobra {

	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}