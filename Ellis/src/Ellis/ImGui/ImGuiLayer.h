#pragma once

#include "Ellis/Core/Core.h"
#include "Ellis/Core/Layer.h"

#include "Ellis/Events/ApplicationEvent.h"
#include "Ellis/Events/KeyEvent.h"
#include "Ellis/Events/MouseEvent.h"

namespace Ellis {

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
	};

}