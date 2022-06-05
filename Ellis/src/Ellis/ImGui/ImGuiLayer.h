#pragma once

#include "Ellis/Core.h"
#include "Ellis/Layer.h"

#include "Ellis/Events/ApplicationEvent.h"
#include "Ellis/Events/KeyEvent.h"
#include "Ellis/Events/MouseEvent.h"

namespace Ellis {

	class ImGuiLayer : public Layer
	{
	private:
		float m_Time = 0.0f;
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiRender() override;

		void Begin();
		void End();
	};

}