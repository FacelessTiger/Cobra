#pragma once

#include "Core.h"
#include "Window.h"
#include "Cobra/Core/LayerStack.h"
#include "Cobra/Events/Event.h"
#include "Cobra/Events/ApplicationEvent.h"

#include "Cobra/Core/Timestep.h"

#include "Cobra/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Cobra {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			CB_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Cobra Application";
		std::string WorkingDirecory;
		ApplicationCommandLineArgs CommandLineArgs;
		bool CustomTitlebar = false;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void SubmitToMainThread(const std::function<void()>& function);

		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }
	private:
		void Run();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void ExecuteMainThreadQueue();
	private:
		static Application* s_Instance;

#if defined(CB_PLATFORM_WINDOWS) && defined(CB_DIST)
		friend int APIENTRY ::WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow);
#else
		friend int ::main(int argc, char** argv);
#endif

		ApplicationSpecification m_Specification;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;

		bool m_Running = true;
		bool m_Minimized = false;

		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;
	};

	// To be defined in client
	Application* CreateApplication(ApplicationCommandLineArgs args);

}