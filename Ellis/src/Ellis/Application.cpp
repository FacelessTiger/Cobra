#include "elpch.h"
#include "Application.h"

#include "Ellis/Events/ApplicationEvent.h"
#include "Ellis/Log.h"

namespace Ellis {
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			EL_INFO(e);
		}
		
		if (e.IsInCategory(EventCategoryInput))
		{
			EL_INFO(e);
		}


		while (true);
	}

}