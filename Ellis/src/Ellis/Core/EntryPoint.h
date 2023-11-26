#pragma once

#include "Ellis/Core/Application.h"

extern Ellis::Application* Ellis::CreateApplication(ApplicationCommandLineArgs args);

#if defined(EL_PLATFORM_WINDOWS) && defined(EL_DIST)
#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Ellis::Log::Init();

	EL_PROFILE_BEGIN_SESSION("Startup", "EllisProfile-Startup.json");
	auto app = Ellis::CreateApplication({ __argc, __argv });
	EL_PROFILE_END_SESSION();

	EL_PROFILE_BEGIN_SESSION("Runtime", "EllisProfile-Runtime.json");
	app->Run();
	EL_PROFILE_END_SESSION();

	EL_PROFILE_BEGIN_SESSION("Shutdown", "EllisProfile-Shutdown.json");
	delete app;
	EL_PROFILE_END_SESSION();

	return 0;
}
#else
int main(int argc, char** argv)
{
	Ellis::Log::Init();

	EL_PROFILE_BEGIN_SESSION("Startup", "EllisProfile-Startup.json");
	auto app = Ellis::CreateApplication({ argc, argv });
	EL_PROFILE_END_SESSION();

	EL_PROFILE_BEGIN_SESSION("Runtime", "EllisProfile-Runtime.json");
	app->Run();
	EL_PROFILE_END_SESSION();

	EL_PROFILE_BEGIN_SESSION("Shutdown", "EllisProfile-Shutdown.json");
	delete app;
	EL_PROFILE_END_SESSION();

	return 0;
}
#endif