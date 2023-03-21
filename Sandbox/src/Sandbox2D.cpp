#include "Sandbox2D.h"

#include <Ellis/Scene/SceneSerializer.h>
#include <Ellis/Utils/PlatformUtils.h>
#include <Ellis/Math/Math.h>
#include <Ellis/Scripting/ScriptEngine.h>
#include <Ellis/Renderer/Font.h>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{ }

void Sandbox2D::OnAttach()
{
	EL_PROFILE_FUNCTION();
	
	if (Ellis::Project::Load("Facerun.eproj"))
	{
		Ellis::ScriptEngine::Init();
		auto startScenePath = Ellis::Project::GetAssetFileSystemPath(Ellis::Project::GetActive()->GetConfig().StartScene);

		m_Scene = Ellis::CreateRef<Ellis::Scene>();
		Ellis::SceneSerializer serializer(m_Scene);

		serializer.Deserialize(startScenePath.string());
	}

	m_Scene->OnRuntimeStart();
}

void Sandbox2D::OnDetach()
{
	EL_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Ellis::Timestep ts)
{
	EL_PROFILE_FUNCTION();

	Ellis::Window& window = Ellis::Application::Get().GetWindow();
	m_Scene->OnViewportResize(window.GetWidth(), window.GetHeight());

	Ellis::Renderer2D::ResetStats();

	//m_Framebuffer->Bind();
	Ellis::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Ellis::RenderCommand::Clear();

	m_Scene->OnUpdateRuntime(ts);
}

void Sandbox2D::OnImGuiRender()
{
    EL_PROFILE_FUNCTION();
}

void Sandbox2D::OnEvent(Ellis::Event& e)
{
	
}