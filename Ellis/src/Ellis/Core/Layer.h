#pragma once

#include "Ellis/Core/Core.h"
#include "Ellis/Core/Timestep.h"
#include "Ellis/Events/Event.h"

namespace Ellis {

	class Layer
	{
	protected:
		std::string m_DebugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnUpdate(Timestep ts) { }
		virtual void OnImGuiRender() { }
		virtual void OnEvent(Event& event) { }

		inline const std::string& GetName() const { return m_DebugName; }
	};

}