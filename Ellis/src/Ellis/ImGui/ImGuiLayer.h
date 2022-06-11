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
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();
	};

}