#include "../../Utils/PlatformUtils.h"
#include "../../Application.h"

#include <windows.h>
#include <shobjidl.h>

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Cobra {

	float Cobra::Time::GetTime()
	{
		return glfwGetTime();
	}

	std::string FileDialogs::OpenFile(const char* filter, GLFWwindow* window)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window(window);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		
		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;

		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter, GLFWwindow* window)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window(window);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);

		if (GetCurrentDirectoryA(256, currentDir))
			ofn.lpstrInitialDir = currentDir;

		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		ofn.lpstrDefExt = strchr(filter, '\0') + 1;

		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;

		return std::string();
	}

	std::string FileDialogs::OpenFolder(GLFWwindow* window)
	{
		IFileOpenDialog* pFileOpen;

		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&pFileOpen);

		pFileOpen->SetOptions(FOS_PICKFOLDERS);
		HRESULT hc = pFileOpen->Show(glfwGetWin32Window(window));

		if (SUCCEEDED(hc))
		{
			IShellItem* pItem;
			pFileOpen->GetResult(&pItem);

			PWSTR filePath;
			pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

			std::wstring ws(filePath);
			return std::string(ws.begin(), ws.end()); // TODO: data loss when outside of ASCII range
		}

		return std::string();
	}

}