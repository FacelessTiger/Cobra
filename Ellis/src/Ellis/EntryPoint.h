#pragma once

#ifdef EL_PLATFORM_WINDOWS

extern Ellis::Application* Ellis::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Ellis::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif