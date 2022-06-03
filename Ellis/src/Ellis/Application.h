#pragma once

#include "Core.h"

namespace Ellis {

	class ELLIS_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client
	Application* CreateApplication();

}