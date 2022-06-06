#pragma once

#include "Core.h"
#include "Window.h"
#include "Ellis/LayerStack.h"
#include "Ellis/Events/Event.h"
#include "Ellis/Events/ApplicationEvent.h"
#include "Ellis/ImGui/ImGuiLayer.h"

#include "Ellis/Renderer/Shader.h"
#include "Ellis/Renderer/Buffer.h"

namespace Ellis {

	class Application
	{
	private:
		static Application* s_Instance;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

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