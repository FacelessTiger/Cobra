#pragma once

#include "Cobra/Core/Core.h"
#include "Cobra/Core/Layer.h"

#include "Cobra/Events/ApplicationEvent.h"
#include "Cobra/Events/KeyEvent.h"
#include "Cobra/Events/MouseEvent.h"

namespace Cobra {

	class ImGuiLayer : public Layer
	{
	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = true;
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	};

}