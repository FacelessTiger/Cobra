#pragma once

#include "Ellis/Core/Window.h"
#include "Ellis/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Ellis {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline void SetTitlebarHovered(bool hovered) override { m_Data.TitlebarHovered = hovered; }
		inline bool IsTitlebarHovered() const override { return m_Data.TitlebarHovered; }
		inline bool IsMaximized() const override { return (bool)glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED); }

		inline void Minimize() override { glfwIconifyWindow(m_Window); }
		inline virtual void Maximize() override { glfwMaximizeWindow(m_Window); }
		inline virtual void Restore() override { glfwRestoreWindow(m_Window); }

		inline void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			bool TitlebarHovered;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}