#include <Ellis.h>

class Sandbox : public Ellis::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Ellis::Application* Ellis::CreateApplication()
{
	return new Sandbox();
}