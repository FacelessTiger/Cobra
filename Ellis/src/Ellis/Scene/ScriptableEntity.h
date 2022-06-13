#pragma once

#include "Ellis/Scene/Entity.h"

namespace Ellis {

	class ScriptableEntity
	{
	private:
		Entity m_Entity;
		friend class Scene;
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() { }
		virtual void OnDestroy() { }
		virtual void OnUpdate(Timestep ts) { }
	};

}