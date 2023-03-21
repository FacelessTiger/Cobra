#pragma once

#include <Ellis.h>

class Sandbox2D : public Ellis::Layer
{
private:
	Ellis::Ref<Ellis::Scene> m_Scene;
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Ellis::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Ellis::Event& e) override;
};