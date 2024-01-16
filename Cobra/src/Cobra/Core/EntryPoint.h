#pragma once

#include "Cobra/Core/Application.h"

extern Cobra::Application* Cobra::CreateApplication(ApplicationCommandLineArgs args);

#if defined(CB_PLATFORM_WINDOWS) && defined(CB_DIST)
#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Cobra::Log::Init();

	CB_PROFILE_BEGIN_SESSION("Startup", "CobraProfile-Startup.json");
	auto app = Cobra::CreateApplication({ __argc, __argv });
	CB_PROFILE_END_SESSION();

	CB_PROFILE_BEGIN_SESSION("Runtime", "CobraProfile-Runtime.json");
	app->Run();
	CB_PROFILE_END_SESSION();

	CB_PROFILE_BEGIN_SESSION("Shutdown", "CobraProfile-Shutdown.json");
	delete app;
	CB_PROFILE_END_SESSION();

	return 0;
}
#else
int main(int argc, char** argv)
{
	Cobra::Log::Init();

	CB_PROFILE_BEGIN_SESSION("Startup", "CobraProfile-Startup.json");
	auto app = Cobra::CreateApplication({ argc, argv });
	CB_PROFILE_END_SESSION();

	CB_PROFILE_BEGIN_SESSION("Runtime", "CobraProfile-Runtime.json");
	app->Run();
	CB_PROFILE_END_SESSION();

	CB_PROFILE_BEGIN_SESSION("Shutdown", "CobraProfile-Shutdown.json");
	delete app;
	CB_PROFILE_END_SESSION();

	return 0;
}
#endif