#include <Ellis.h>
#include <Ellis/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Ellis {

	class EllisEditor : public Application
	{
	public:
		EllisEditor()
			: Application("Ellis Editor")
		{
			PushLayer(new EditorLayer());
		}

		~EllisEditor()
		{ }
	};

	Application* CreateApplication()
	{
		return new EllisEditor();
	}

}