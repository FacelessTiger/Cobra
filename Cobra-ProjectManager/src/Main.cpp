#include "Application.h"
#include "PlatformDetection.h"

#if defined(CB_PLATFORM_WINDOWS) && defined(CB_DIST)
#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#else
int main()
#endif
{
	Cobra::Application app;
	app.Run();

	return 0;
}