#pragma once

#include "Cobra/Core/Core.h"
#include "Cobra/Core/Timestep.h"
#include "Cobra/Events/Event.h"

namespace Cobra {

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