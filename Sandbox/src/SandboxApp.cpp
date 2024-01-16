#include <Cobra.h>
#include <Cobra/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Cobra::Application
{
public:
	Sandbox(const Cobra::ApplicationSpecification& specification)
		: Cobra::Application(specification)
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{ }
};

Cobra::Application* Cobra::CreateApplication(Cobra::ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	//spec.WorkingDirecory = "../Cobra-Editor";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}