#pragma once

#include "Core.h"
#include "Window.h"
#include "Ellis/LayerStack.h"
#include "Ellis/Events/Event.h"
#include "Ellis/Events/ApplicationEvent.h"
#include "Ellis/ImGui/ImGuiLayer.h"

#include "Ellis/Renderer/Shader.h"
#include "Ellis/Renderer/Buffer.h"
#include "Ellis/Renderer/VertexArray.h"

namespace Ellis {

	class Application
	{
	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Shader> m_Shader;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_BlueShader;

		bool m_Running = true;
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	// To be defined in client
	Application* CreateApplication();

}