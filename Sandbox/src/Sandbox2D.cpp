#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{ }

void Sandbox2D::OnAttach()
{
	EL_PROFILE_FUNCTION();

	m_CheckerboardTexture = Ellis::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{ 
	EL_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Ellis::Timestep ts)
{
	EL_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	// Render
	{
		EL_PROFILE_SCOPE("Renderer Prep");
		Ellis::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Ellis::RenderCommand::Clear();
	}

	{
		EL_PROFILE_SCOPE("Renderer Draw");
		Ellis::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Ellis::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Ellis::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Ellis::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Ellis::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	EL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Ellis::Event& e)
{
	m_CameraController.OnEvent(e);
}