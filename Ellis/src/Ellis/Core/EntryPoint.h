#pragma once

#ifdef EL_PLATFORM_WINDOWS

extern Ellis::Application* Ellis::CreateApplication();

int main(int argc, char** argv)
{
	Ellis::Log::Init();
	EL_CORE_WARN("Initialized log!");
	int a = 5;
	EL_INFO("Hello! Var={0}", a);

	auto app = Ellis::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif