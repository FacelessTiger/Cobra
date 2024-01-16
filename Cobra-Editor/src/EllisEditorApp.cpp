#include <Cobra.h>
#include <Cobra/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Cobra {

	class CobraEditor : public Application
	{
	public:
		CobraEditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}

		~CobraEditor()
		{ }
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "Cobra Editor";
		spec.CommandLineArgs = args;
		spec.CustomTitlebar = true;

		return new CobraEditor(spec);
	}

}