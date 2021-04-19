#include "WinMain.h"

int WINAPI wWinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PWSTR		pCmdLine,
	int			nCmdShow
)
{
	WinMain::Start(hInstance, hPrevInstance, pCmdLine, nCmdShow);
}