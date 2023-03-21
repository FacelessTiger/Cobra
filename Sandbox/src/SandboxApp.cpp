#include <Ellis.h>
#include <Ellis/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Ellis::Application
{
public:
	Sandbox(const Ellis::ApplicationSpecification& specification)
		: Ellis::Application(specification)
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{ }
};

Ellis::Application* Ellis::CreateApplication(Ellis::ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	//spec.WorkingDirecory = "../Ellis-Editor";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}