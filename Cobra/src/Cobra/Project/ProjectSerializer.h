#pragma once

#include "Project.h"

namespace Cobra {

	class ProjectSerializer
	{
	private:
		Ref<Project> m_Project;
	public:
		ProjectSerializer(Ref<Project> project);

		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);
	};

}