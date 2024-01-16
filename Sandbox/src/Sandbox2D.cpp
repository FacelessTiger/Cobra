#include "Sandbox2D.h"

#include <Cobra/Scene/SceneSerializer.h>
#include <Cobra/Utils/PlatformUtils.h>
#include <Cobra/Math/Math.h>
#include <Cobra/Scripting/ScriptEngine.h>
#include <Cobra/Renderer/Font.h>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{ }

void Sandbox2D::OnAttach()
{
	CB_PROFILE_FUNCTION();
	
	if (Cobra::Project::Load("Facerun.eproj"))
	{
		Cobra::ScriptEngine::Init();
		auto startScenePath = Cobra::Project::GetAssetFileSystemPath(Cobra::Project::GetActive()->GetConfig().StartScene);

		m_Scene = Cobra::CreateRef<Cobra::Scene>();
		Cobra::SceneSerializer serializer(m_Scene);

		serializer.Deserialize(startScenePath.string());
	}

	m_Scene->OnRuntimeStart();
}

void Sandbox2D::OnDetach()
{
	CB_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Cobra::Timestep ts)
{
	CB_PROFILE_FUNCTION();

	Cobra::Window& window = Cobra::Application::Get().GetWindow();
	m_Scene->OnViewportResize(window.GetWidth(), window.GetHeight());

	Cobra::Renderer2D::ResetStats();

	//m_Framebuffer->Bind();
	Cobra::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Cobra::RenderCommand::Clear();

	m_Scene->OnUpdateRuntime(ts);
}

void Sandbox2D::OnImGuiRender()
{
    CB_PROFILE_FUNCTION();
}

void Sandbox2D::OnEvent(Cobra::Event& e)
{
	
}