#include <Ellis.h>
#include <imgui/imgui.h>

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

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::End();
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
	}

	~Sandbox()
	{

	}
};

Ellis::Application* Ellis::CreateApplication()
{
	return new Sandbox();
}