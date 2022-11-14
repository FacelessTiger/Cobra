#pragma once

#include <Ellis/Core/Buffer.h>

namespace Ellis {

	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}