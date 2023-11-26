#include <Ellis.h>
#include <Ellis/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Ellis {

	class EllisEditor : public Application
	{
	public:
		EllisEditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}

		~EllisEditor()
		{ }
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Ellis Editor";
		spec.CommandLineArgs = args;
		spec.CustomTitlebar = true;

		return new EllisEditor(spec);
	}

}