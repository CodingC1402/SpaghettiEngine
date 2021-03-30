#include "App.h"

//Entry point
int WINAPI wWinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PWSTR		pCmdLine,
	int			nCmdShow
)
{
	App app;
	app.ShowExtraInfo();
	BOOL result = app.Go();
	return result;
}