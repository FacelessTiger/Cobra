#include <Ellis.h>
#include <Ellis/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Ellis::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{ }
};

Ellis::Application* Ellis::CreateApplication()
{
	return new Sandbox();
}