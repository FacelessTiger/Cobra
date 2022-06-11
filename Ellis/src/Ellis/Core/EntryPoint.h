#pragma once

#ifdef EL_PLATFORM_WINDOWS

extern Ellis::Application* Ellis::CreateApplication();

int main(int argc, char** argv)
{
	Ellis::Log::Init();

	EL_PROFILE_BEGIN_SESSION("Startup", "EllisProfile-Startup.json");
	auto app = Ellis::CreateApplication();
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