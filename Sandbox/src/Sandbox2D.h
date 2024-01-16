#pragma once

#include <Cobra.h>

class Sandbox2D : public Cobra::Layer
{
private:
	Cobra::Ref<Cobra::Scene> m_Scene;
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Cobra::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Cobra::Event& e) override;
};