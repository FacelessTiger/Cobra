#include <Ellis.h>
#include <Ellis/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Ellis {

	class EllisEditor : public Application
	{
	public:
		EllisEditor(ApplicationCommandLineArgs args)
			: Application("Ellis Editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~EllisEditor()
		{ }
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new EllisEditor(args);
	}

}