#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <Ellis.h>

class ExampleLayer : public Ellis::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{ }

	void OnUpdate() override
	{
		if (Ellis::Input::IsKeyPressed(EL_KEY_TAB))
			EL_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(Ellis::Event& event) override
	{
		if (event.GetEventType() == Ellis::EventType::KeyPressed)
		{
			Ellis::KeyPressedEvent& e = (Ellis::KeyPressedEvent&)event;
			if (e.GetKeyCode() == EL_KEY_TAB)
				EL_TRACE("Tab key is pressed (event)!");

			EL_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Ellis::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Ellis::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Ellis::Application* Ellis::CreateApplication()
{
	return new Sandbox();
}