#include <Ellis.h>

class ExampleLayer : public Ellis::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{ }

	void OnUpdate() override
	{
		EL_INFO("ExampleLayer::Update");
	}

	void OnEvent(Ellis::Event& event) override
	{
		EL_INFO("{0}", event);
	}
};

class Sandbox : public Ellis::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Ellis::Application* Ellis::CreateApplication()
{
	return new Sandbox();
}