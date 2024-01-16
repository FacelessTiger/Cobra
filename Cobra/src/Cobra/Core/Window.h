#pragma once

#include "cbpch.h"

#include "Cobra/Core/Core.h"
#include "Cobra/Events/Event.h"

namespace Cobra {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool CustomTitlebar;

		WindowProps(const std::string& title = "Cobra Engine",
			uint32_t width = 1600,
			uint32_t height = 900,
			bool customTitlebar = false)
			: Title(title), Width(width), Height(height), CustomTitlebar(customTitlebar)
		{ }
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetTitlebarHovered(bool hovered) = 0;
		virtual bool IsTitlebarHovered() const = 0;
		virtual bool IsMaximized() const = 0;

		virtual void Minimize() = 0;
		virtual void Maximize() = 0;
		virtual void Restore() = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}