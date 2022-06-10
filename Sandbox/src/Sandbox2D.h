#pragma once

#include <Ellis.h>

class Sandbox2D : public Ellis::Layer
{
private:
	Ellis::OrthographicCameraController m_CameraController;

	// Temp
	Ellis::Ref<Ellis::Shader> m_FlatColorShader;
	Ellis::Ref<Ellis::VertexArray> m_SquareVA;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Ellis::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Ellis::Event& e) override;
};