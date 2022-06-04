#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Ellis/Events/ApplicationEvent.h"
#include "Window.h"

namespace Ellis {

	class ELLIS_API Application
	{
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	// To be defined in client
	Application* CreateApplication();

}