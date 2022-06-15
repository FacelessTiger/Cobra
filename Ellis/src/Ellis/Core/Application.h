#pragma once

#include "Core.h"
#include "Window.h"
#include "Ellis/Core/LayerStack.h"
#include "Ellis/Events/Event.h"
#include "Ellis/Events/ApplicationEvent.h"

#include "Ellis/Core/Timestep.h"

#include "Ellis/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Ellis {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			EL_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class Application
	{
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		ApplicationCommandLineArgs m_CommandLineArgs;

		float m_LastFrameTime = 0.0f;

		bool m_Running = true;
		bool m_Minimized = false;
	public:
		Application(const std::string& name = "Ellis App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
	private:
		void Run();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	};

	// To be defined in client
	Application* CreateApplication(ApplicationCommandLineArgs args);

}